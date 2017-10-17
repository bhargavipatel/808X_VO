/** 
 *  @file    allmod.cpp
 *  @author  Bhargavi Patel
 *  @copyright MIT License
 *  
 *  @brief ENPM808X, MidSem Exam
 *  @section DESCRIPTION 
 *  This program is used to check the result of the visual 
 *  odomentry code and see if it is working properly
 */

#include <allmod.hpp>

/**
 * @brief      Constructs the object.
 */
allmod::allmod() {
}
/**
 * @brief      Destroys the object.
 */
allmod::~allmod() {}

bool allmod::checkRes() {
  return (vo_.checkRes());  //
}
