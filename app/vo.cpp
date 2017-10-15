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

    printf("try this");
    flagval = true;
    string imgFolder = "/home/bhargavi/Documents/SDR/Copy_Exam_808X/data/Oxford_dataset/stereo/centre/";
    string mdlFolder = "/home/bhargavi/Documents/SDR/Copy_Exam_808X/data/Oxford_dataset/model/";
    printf("flodername",imgFolder);
    Mat cur, nxt;
    Mat R_f, t_f;
    vector<string> imgList;

    createImageList(imgList, imgFolder);
   
    //Starting from a particular image
    int i = 0;
    string img1 = imgFolder + "1399381446204705.png";
    while (imgList[i] != "1399381446204705.png")
        i++;
    cout << imgList[i] << " and " << imgList[i+1] << endl;

    //Reading start image and demosaicing it
    Mat tmp = imread(img1, IMREAD_GRAYSCALE);
    int r = tmp.rows;
    int c = tmp.cols;

    Mat Mat1;
    cvtColor(tmp, Mat1, COLOR_BayerGR2BGR);
    //Reading LUT file and creating camera matrix
    string lut_file = mdlFolder + "stereo_narrow_left_distortion_lut.bin";
    Mat lutMat;
    readLutToMat(lutMat, lut_file, c, r);
    lutMat.convertTo(lutMat, CV_32FC2);

    Mat lutxy[2];
    split(lutMat, lutxy);

    Mat cameraMat = ((Mat1d(3,3) << 964.828979, 0, 643.788025, 0, 964.828979, 484.407990, 0, 0, 1));

    //R_p, t_p represent previous poses starting with R_p = eye(3,3) and t_p = zeros(3,1)
    Mat R_p, t_p;
    R_p = Mat::eye(3, 3, CV_64F);
    t_p = Mat::zeros(3, 1, CV_64F);

    //Offset and scale to translational prarameters while plotting trajectory
    int off_set = 150;
    int scale = 100;

    //Variables needed for plotting
    Point p1, p2;
    int x, y, z;
    x = int(t_p.at<double>(0)*scale) + off_set;
    y = int(t_p.at<double>(1)*scale) + off_set;
    z = int(t_p.at<double>(2)*scale) + off_set;
    p1 = Point(x, y);
    Mat plotImg = Mat::zeros(600, 600, CV_8UC3);

    //Undistort start image
    remap(Mat1, Mat1, lutxy[0], lutxy[1], INTER_LINEAR, BORDER_CONSTANT);

    //Create SURF detector
    int minHessian = 400;
    Ptr<SURF> detector = SURF::create( minHessian );

    vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;

    detector->detectAndCompute( Mat1, Mat(), keypoints_1, descriptors_1 );

    for (int j = i+1; j < i + 100; ++j) {

        //Read new image
        string img2 = imgFolder+imgList[j];
        Mat tmp1 = imread(img2, IMREAD_GRAYSCALE);
        Mat Mat2;
        cvtColor(tmp1, Mat2, COLOR_BayerGR2BGR);

        //Undistort image
        remap(Mat2, Mat2, lutxy[0], lutxy[1], INTER_LINEAR, BORDER_CONSTANT);

        detector->detectAndCompute( Mat2, Mat(), keypoints_2, descriptors_2 );
        vector< DMatch > good_matches;
        computeGoodMatches(good_matches, descriptors_1, descriptors_2);
        
        //Draw keypoints
        //Mat img_keypoints1, img_keypoints2, img_matches;

        //drawMatches( Mat1, keypoints_1, Mat2, keypoints_2, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
        //        vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

        vector<Point2f> obj, scene;
        for (size_t i = 0; i < good_matches.size(); ++i) {
            obj.push_back( keypoints_1[ good_matches[i].queryIdx ].pt );
            scene.push_back( keypoints_2[ good_matches[i].trainIdx ].pt );
        }

        double focal = cameraMat.at<double>(0, 0);
        Point2d pp( cameraMat.at<double>(0, 2), cameraMat.at<double>(1, 2) );
        Mat E, R, t, mask;

        cout << "Test\n";


        E = findEssentialMat(scene, obj, focal, pp, RANSAC, 0.999, 1.0, mask);
        recoverPose(E, scene, obj, R, t, focal, pp, mask);
        //cout << "Size of t is " << t.rows << ", " << t.cols << endl;
        //cout << "Translational parameters are t[0] = " << t.at<double>(0) << ", t[1] = " << t.at<double>(1) << " and t[2] = " << t.at<double>(2) << endl;
        //cout << "Translational parameters are x = " << x << ", y = " << y << " and z = " << z << endl;

        //imshow( "Good Matches", img_matches );

        t_p = t_p + (R_p*t);
        R_p = R*R_p;
        cout << "Translational parameters are t[0] = " << t.at<double>(0) << ", t[1] = " << t.at<double>(1) << " and t[2] = " << t.at<double>(2) << endl;

        x = int(t_p.at<double>(0)*scale) + off_set;
        y = int(t_p.at<double>(1)*scale) + off_set;
        z = int(t_p.at<double>(2)*scale) + off_set;
        p2 = Point(x, y);

        line(plotImg, p1, p2, Scalar(0, 0, 255));
        imshow("Image Feed", Mat2);
        imshow("Trajectory", plotImg);
        waitKey(10);
        Mat1 = Mat2;
        p1 = p2;
        keypoints_1 = keypoints_2;
        descriptors_1 = descriptors_2;
    }

    waitKey(0);
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
void vo::createImageList(vector<string>& imgList, string imgFolder) {
  printf("imglist print 1",imgList);
  printf("img folder print 2",imgFolder);
  DIR *dp;
    struct dirent *dirp;
    dp = opendir(imgFolder.c_str());
    printf("printing dp 3",dp);
    printf("printing add dp 4",&dp);

    while ( (dirp = readdir(dp)) != NULL )
        printf("printing dirp 5",dirp);
        printf("printing add dirp 6",&dirp);
        imgList.push_back(dirp->d_name);
        printf("before close 7");
    closedir(dp);
    printf("after close 8");

    sort(imgList.begin(), imgList.end());
    printf("after sort 9");
}

/**
 * @brief      convert image from LUT to Mat
 *
 * @param[in]  input  
 */
int vo::readLutToMat(Mat &I, string path, int matWidth, int matHeight){
    //declare values to be written
    float fvalue;
    double dvalue;
 
    //create the file stream
    ifstream file(path.c_str(), ios::in | ios::binary );
    if (!file)
        return -1;
 
    //change Mat type
    I = Mat::zeros(matHeight*matWidth*2,1,CV_64F);
 
    for (int i=0; i < matWidth*matHeight*2; ++i) {
        file.read((char*) &dvalue, sizeof(dvalue));
        I.at<double>(i) = dvalue;
    }
 
    //close file
    file.close();

    I = I.reshape(1, 2);
    transpose(I,I);
    I = I.reshape(2, matHeight);
 
    return 0;

}
/**
 * @brief      Compare features and do thresholding
 *
 * @param[in]  input  
 */


void vo::computeGoodMatches(vector< DMatch >& good_matches, Mat& descriptors_1, Mat& descriptors_2) {
    FlannBasedMatcher matcher;
    vector< DMatch > matches;
    matcher.match( descriptors_1, descriptors_2, matches );

    double max_dist = 0, min_dist = 100;

    // Calculating min_dist and max_dist between keypoints
    for (int i = 0; i < descriptors_1.rows; ++i) {
        double dist = matches[i].distance;
        if ( dist < min_dist )  min_dist = dist;
        if ( dist > max_dist )  max_dist = dist;
    }

    double dist_thresh = max(2*min_dist, 0.02);
    for (int i = 0; i < descriptors_1.rows; ++i) {
        if (matches[i].distance <= dist_thresh)
            good_matches.push_back(matches[i]);
    }
}

bool vo::checkRes() {
  printf("Before flag value",flagval);  
  return flagval;  // Return flag value
}