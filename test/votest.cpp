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
 * @param[in]  ResultCheck Name of the test
 */
TEST_F(VisualTest, ResultCheck) {
  EXPECT_EQ(voObj.checkRes(), 1);
}
