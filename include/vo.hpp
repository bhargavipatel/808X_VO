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

#ifndef VO_HPP_
#define VO_HPP_

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <getdata.hpp>


#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

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
 	getdata getdataobj_;
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
	int readLutToMat(Mat &I, string path, int matWidth, int matHeight);

	/**
	 * @brief      for good matches
	 */

	void computeGoodMatches(vector< DMatch >& good_matches, Mat& descriptors_1, Mat& descriptors_2);
 
	/**
	 * @brief      Get Results.
	 *
	 * @return     The 
	 */
	bool checkRes(); 
	bool checkfile_fun(); 
};
#endif  // VO_HPP_