/** 
 *  @file    allmod.hpp
 *  @author  Bhargavi Patel
 *  @copyright MIT License
 *  
 *  @brief ENPM808X, Midsem Exam
 *  @section DESCRIPTION
 *  
 *  
 *
 */


#ifndef ALLMOD_HPP_
#define ALLMOD_HPP_

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

#endif  // ALLMOD_HPP_