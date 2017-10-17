/** 
 *  @file    vo.hpp
 *  @author  Bhargavi Patel
 *  @copyright MIT License
 *  
 *  @brief ENPM808X, MidSem Exam
 *
 *  @section DESCRIPTION
 *   *
 */
#ifndef INCLUDE_VO_HPP_
#define INCLUDE_VO_HPP_
#include <dirent.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <memory>
#include "getdata.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
// using namespace std;
// using namespace cv;
// using namespace cv::xfeatures2d;

/**
 * @brief      Class for vo module.
 */
class vo {
 private:
    bool flagval = false;
    bool filecheck = false;

 public:
    /**
     * @brief      Constructor
     */
    // getdata getdataobj_;
    getdata getdataobj_;
    // cv::Mat tmp;
    std::string mdlFolder = "../data/Oxford_dataset/model/";
    cv::Mat cur, nxt;
    cv::Mat R_f, t_f;
    std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
    cv::Mat descriptors_1, descriptors_2;

    // std
    vo();

    /**
     * @brief      Destroys the object.
     */
    ~vo();

    /**
     * @brief      Sets
     *
     * @param[in]  input  
     */
    // void createImageList(vector<String>& imgList, String fun_imgFolder);

    /**
     * @brief      Gets 
     *
     * @return     The 
     */

    /**
     * @brief      readLutToMat
     */
    int readLutToMat(cv::Mat&I, cv::String path, int matWidth, int matHeight);
    /**
     * @brief      for good matches
     */
    void computeGoodMatches(std::vector< cv::DMatch >& good_matches,
          cv::Mat& descriptors_1, cv::Mat& descriptors_2);
    /**
     * @brief      Get Results.
     *
     * @return     The 
     */
    bool checkRes();
    bool checkfile_fun();
};
#endif  //  INCLUDE_VO_HPP_
