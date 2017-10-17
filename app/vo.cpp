/** 
 *  @file    vo.cpp
 *  @author  Bhargavi Patel
 *  @copyright MIT License
 *  
 *  @brief ENPM808X, MidSem Exam
 *
 *  @section DESCRIPTION
 *   *
 */

#include <vo.hpp>

/**
 * @brief      Constructs the object.
 */
vo::vo() {
    // getdataobj_ = std::make_unique<getdata>();

    // std::string dumval = getdataobj_.dummyvar;
    // printf("%s\n",dumval);

    printf("try this");
    flagval = true;
    filecheck = true;
    // std::string imgFolder = getdataobj_.imgFolder1;
    // cv::String fun_imgFolder = getdataobj_.fun_imgFolder1;
    // std::string mdlFolder = getdataobj_.mdlFolder1;
    // getdataobj_.fun_imgFolder = fun_imgFolder;
    // getdataobj_.mdlFolder = mdlFolder;
    // std::string imgFolder = "../data/Oxford_dataset/stereo/centre/";
    cv::String fun_imgFolder = "../data/Oxford_dataset/stereo/centre/*.png";
    std::string mdlFolder = "../data/Oxford_dataset/model/";
    printf("Object thingy worked: ");
    // printf("%s\n",imgFolder.c_str());
    // printf(imgFolder);
    printf("before function");
    cv::Mat cur, nxt;
    cv::Mat R_f, t_f;
    std::vector<cv::String> imgList;
    // createImageList(imgList, fun_imgFolder);
    printf("creatimage function");
    glob(fun_imgFolder, imgList);
    sort(imgList.begin(), imgList.end());
    // Starting from a particular image
    int i = 0;
    // string img1 = imgFolder + "1399381446204705.png";
    // while (imgList[i] != "1399381446204705.png")
    //     i++;
    // cout << imgList[i] << " and " << imgList[i+1] << endl;
    if (imgList.empty() == 1)
        filecheck = false;
    // Reading start image and demosaicing it
    cv::String img1 = imgList[1];
    cv::Mat tmp = imread(img1, cv::IMREAD_GRAYSCALE);
    
    int r = tmp.rows;
    int c = tmp.cols;

    cv::Mat Mat1;
    cvtColor(tmp, Mat1, cv::COLOR_BayerGR2BGR);
    // Reading LUT file and creating camera matrix
    std::string lut_file = mdlFolder + "stereo_narrow_left_distortion_lut.bin";
    cv::Mat lutMat;
    readLutToMat(lutMat, lut_file, c, r);
    lutMat.convertTo(lutMat, CV_32FC2);

    cv::Mat lutxy[2];
    split(lutMat, lutxy);
    cv::Mat cameraMat = ((cv::Mat1d(3, 3) << 964.828979,
       0, 643.788025, 0, 964.828979, 484.407990, 0, 0, 1));

    // R_p, t_p represent previous poses starting
    // with R_p = eye(3,3) and t_p = zeros(3,1)
    cv::Mat R_p, t_p;
    R_p = cv::Mat::eye(3, 3, CV_64F);
    t_p = cv::Mat::zeros(3, 1, CV_64F);

    // Offset and scale to translational prarameters while plotting trajectory
    int off_set = 150;
    int scale = 100;

    // Variables needed for plotting
    cv::Point p1, p2;
    int x, y;
    // int z;
    x = static_cast<int>(t_p.at<double>(0)*scale) + off_set;
    y = static_cast<int>(t_p.at<double>(1)*scale) + off_set;
    // z = static_cast<int>(t_p.at<double>(2)*scale) + off_set;
    p1 = cv::Point(x, y);
    cv::Mat plotImg = cv::Mat::zeros(600, 600, CV_8UC3);

    // Undistort start image
    remap(Mat1, Mat1, lutxy[0], lutxy[1],
       cv::INTER_LINEAR, cv::BORDER_CONSTANT);

    // Create SURF detector
    int minHessian = 400;
    cv::Ptr<cv::xfeatures2d::SURF> detector =
       cv::xfeatures2d::SURF::create(minHessian);

    std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
    cv::Mat descriptors_1, descriptors_2;

    detector->detectAndCompute(Mat1, cv::Mat(), keypoints_1, descriptors_1);

    for (int j = i+1; j < i + 100; ++j) {
        // Read new image
        // string img2 = imgFolder+imgList[j];
        cv::String img2 = imgList[j];
        cv::Mat tmp1 = imread(img2, cv::IMREAD_GRAYSCALE);
        cv::Mat Mat2;
        cvtColor(tmp1, Mat2, cv::COLOR_BayerGR2BGR);
        // Undistort image
        remap(Mat2, Mat2, lutxy[0], lutxy[1],
              cv::INTER_LINEAR, cv::BORDER_CONSTANT);

        detector->detectAndCompute(Mat2, cv::Mat(),
           keypoints_2, descriptors_2);
        std::vector< cv::DMatch > good_matches;
        computeGoodMatches(good_matches, descriptors_1, descriptors_2);
        // Draw keypoints
        // Mat img_keypoints1, img_keypoints2, img_matches;

        // drawMatches( Mat1, keypoints_1, Mat2, keypoints_2,
        // good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
        //        vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

        std::vector<cv::Point2f> obj, scene;
        for (size_t i = 0; i < good_matches.size(); ++i) {
            obj.push_back(keypoints_1[ good_matches[i].queryIdx ].pt);
            scene.push_back(keypoints_2[ good_matches[i].trainIdx ].pt);
        }
        double focal = cameraMat.at<double>(0, 0);
        cv::Point2d pp(cameraMat.at<double>(0, 2), cameraMat.at<double>(1, 2));
        cv::Mat E, R, t, mask;

        std::cout << "Test\n";
        E = findEssentialMat(scene, obj, focal, pp,
           cv::RANSAC, 0.999, 1.0, mask);
        recoverPose(E, scene, obj, R, t, focal, pp, mask);
        // cout << "Size of t is " << t.rows << ", " << t.cols << endl;
        // cout << "Translational parameters are t[0] =
        // " << t.at<double>(0) << ", t[1] = " << t.at<double>(1)
        // << " and t[2] = " << t.at<double>(2) << endl;
        // cout << "Translational parameters are x = "
        // << x << ", y = " << y << " and z = " << z << endl;
        // imshow( "Good Matches", img_matches );
        t_p = t_p + (R_p*t);
        R_p = R*R_p;
        std::cout << "Translational parameters are t[0] = " << t.at<double>(0)
          << ", t[1] = " << t.at<double>(1) << " and t[2] = " << t.at<double>(2)
             << std::endl;
        x = static_cast<int>(t_p.at<double>(0)*scale) + off_set;
        y = static_cast<int>(t_p.at<double>(1)*scale) + off_set;
        // z = static_cast<int>(t_p.at<double>(2)*scale) + off_set;
        p2 = cv::Point(x, y);

        line(plotImg, p1, p2, cv::Scalar(0, 0, 255));
        // imshow("Image Feed", Mat2);
        // imshow("Trajectory", plotImg);
        cv::waitKey(10);
        Mat1 = Mat2;
        p1 = p2;
        keypoints_1 = keypoints_2;
        descriptors_1 = descriptors_2;
    }
    // waitKey(0);
}

/**
 * @brief      Destroys the object.
 */
vo::~vo() {}
/**
 * @brief      Creation of Image List
 *
 * @param[in]  input  
 */
// void vo::createImageList(vector<String>& imgList, String fun_imgFolder) {
//   printf("creatimage function");
//   glob(fun_imgFolder,imgList);


//   // printf("imglist print 1");
//   // printf("img folder print 2");
//   // DIR *dp;
//   //   struct dirent *dirp;
//   //   dp = opendir(imgFolder.c_str());
//   //   printf("printing dp 3");
//   //   printf("printing add dp 4");

//   //   while ( (dirp = readdir(dp)) != NULL )
//   //       printf("printing dirp 5",dirp);
//   //       printf("printing add dirp 6",&dirp);
//   //       imgList.push_back(dirp->d_name);
//   //       printf("before close 7");
//   //   closedir(dp);
//   //   printf("after close 8");

//     sort(imgList.begin(), imgList.end());
//    // printf("after sort 9");
// }

/**
 * @brief      convert image from LUT to Mat
 *
 * @param[in]  input  
 */
int vo::readLutToMat(cv::Mat&I, cv::String path, int matWidth, int matHeight) {
    // declare values to be written
    // float fvalue;
    double dvalue;
    // create the file stream
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file)
        return -1;
    // change Mat type
    I = cv::Mat::zeros(matHeight*matWidth*2, 1, CV_64F);
    for (int i=0; i < matWidth*matHeight*2; ++i) {
        // reinterpret_cast<char*>(...) instead  [readability/casting]
        // file.read((char*) &dvalue, sizeof(dvalue));
        file.read(reinterpret_cast<char*>(&dvalue), sizeof(dvalue));
        // file.read((char*) &dvalue, sizeof(dvalue));
        I.at<double>(i) = dvalue;
    }
    // close file
    file.close();
    I = I.reshape(1, 2);
    transpose(I, I);
    I = I.reshape(2, matHeight);
    return 0;
}
/**
 * @brief      Compare features and do thresholding
 *
 * @param[in]  input  
 */


void vo::computeGoodMatches(std::vector< cv::DMatch >& good_matches,
      cv::Mat& descriptors_1, cv::Mat& descriptors_2) {
    // using namespace cv;
    cv::FlannBasedMatcher matcher;
    std::vector< cv::DMatch > matches;
    matcher.match(descriptors_1, descriptors_2, matches);

    double max_dist = 0, min_dist = 100;

    // Calculating min_dist and max_dist between keypoints
    for (int i = 0; i < descriptors_1.rows; ++i) {
        double dist = matches[i].distance;
        if ( dist < min_dist )  min_dist = dist;
        if ( dist > max_dist )  max_dist = dist;
    }

    double dist_thresh = std::max(2*min_dist, 0.02);
    for (int i = 0; i < descriptors_1.rows; ++i) {
        if (matches[i].distance <= dist_thresh)
            good_matches.push_back(matches[i]);
    }
}

bool vo::checkRes() {
  printf("Before flag value");
  return flagval;  // Return flag value
}

bool vo::checkfile_fun() {
  printf("Filecontant value");
  return filecheck;  // Return flag value
}
