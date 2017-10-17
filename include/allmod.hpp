/** 
 *  @file    allmod.hpp
 *  @author  Bhargavi Patel
 *  @copyright MIT License
 *  
 *  @brief ENPM808X, Midsem Exam
 *  @section DESCRIPTION 
 *  This program checks and integrate the main visual odometry
 *  module vo.
 *
 */
#ifndef INCLUDE_ALLMOD_HPP_
#define INCLUDE_ALLMOD_HPP_
#include <iostream>
#include <vector>
#include "vo.hpp"

/**
 * @brief      Class for allmod.
 */
class allmod {
 private:
  vo vo_;
 public:
  /**
   * @brief      Constructor
   */
  allmod();

  /**
   * @brief      Destroys the object.
   */
  ~allmod();

  /**
   * @brief      Gets 
   *
   * @return     The 
   */
  bool checkRes();
};

#endif  // INCLUDE_ALLMOD_HPP_
