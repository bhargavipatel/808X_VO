/** 
 *  @file    votest.cpp
 *  @author  Bhargavi Patel
 *  @copyright MIT License
 *  
 *  @brief ENPM808X, MidSem Exam
 *
 *  @section DESCRIPTION
 *  
 *  This 
 *
 */

#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "vo.hpp"

class VisualTest : public ::testing::Test {
 protected:
    vo voObj;  // Create an object to be used in the tests
    std::string val1 = "../data/Oxford_dataset/model/";
};

/**
 * @brief      Test for Sanity, just checks whether
 * everything is in order.
 */
TEST_F(VisualTest, SanityCheck) {
  EXPECT_EQ(1, 1);
}

/**
 * @brief      Result test
 *
 * @param[in]  VisualTest vo object
 * @param[in]  ResultCheck is the main test to check if 
 * the visual odometry is providing proper output and 
 * all the functions are working in order.
 */
TEST_F(VisualTest, ResultCheck) {
  EXPECT_EQ(voObj.checkRes(), 1);
}

/**
 * @brief      File Check test
 *
 * @param[in]  VisualTest vo object
 * @param[in]  ResultCheck Name of the test
 */
TEST_F(VisualTest, FileCheck) {
  EXPECT_EQ(voObj.checkfile_fun(), 1);
}

/**
 * @brief       Check test
 *
 * @param[in]  VisualTest vo object
 * @param[in]  This Test is checking if the folder
 * is loading properly and file are being read.
 */
TEST_F(VisualTest, checkFolder) {
  std::string val2 = voObj.mdlFolder;
  EXPECT_EQ(val1, val2);
}
