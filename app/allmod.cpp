/** 
 *  @file    allmod.cpp
 *  @author  Bhargavi Patel
 *  @copyright MIT License
 *  
 *  @brief ENPM808X, MidSem Exam
 *  @section DESCRIPTION
 *  
 *  
 */

#include <allmod.hpp>

/**
 * @brief      Constructs the object.
 */
allmod::allmod(){
}
/**
 * @brief      Destroys the object.
 */
allmod::~allmod() {}

bool allmod::checkRes() {    
  return (vo_.checkRes()); //  
}
