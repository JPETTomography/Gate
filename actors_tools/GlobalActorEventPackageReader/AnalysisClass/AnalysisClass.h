/**
 *  @copyright Copyright 2019 The J-PET Gate Authors. All rights reserved.
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  @file AnalysisClass.h
 */
#ifndef AnalysisClass_h
#define AnalysisClass_h
#include <string>
#include "../Dictionaries/Dictionaries.h"

/**
 * @About class: This class represents the analisis class - this means this class has to contain all analysis logic of data from simulation.
 * Analysis class is informed about next event via method noticeEvent which is called by Reader - so the user doesn't have to care about loading next events from file.
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class AnalysisClass
{
 public:
  AnalysisClass();
  virtual ~AnalysisClass();

  /** This class is a core of analysis - here the user defines whole logic how to analyse data from event.
   * @param: event - an event received from Reader for analysis
  **/
  virtual void noticeEvent( const Dictionaries::Event& event ) = 0;

  /** If the user wants to send extra parameters for his class then is necessary to implement this method - remember to start reading data from argv[>= 3]
  **/
  virtual void readAdditionalArguments( int argc, char* argv[] );

  /** If the user wants to display at the end of analysis some information then is necessary to implement this method
  **/
  virtual void displayStatus() const; 

  /** If the user wants to save any data (e.g. histograms to file) then is is necessary to implement this method
  **/
  virtual void saveData();
};

/** This how we register analysis class to AnalysisClassesManager.
  * This is necessary to define in your analysis class a additional constructor:
  * YourAnalysisClass( std::string class_name ); // <--- this in *.h file
  * YourAnalysisClass::YourAnalysisClass( std::string class_name ) { AnalysisClassesManager::getInstance()->registerClass( class_name, new YourAnalysisClass ); }
**/
#define REGISTER_ANALYSIS_CLASS(NAME,CLASS)				\
  static CLASS registered_CLASS(#NAME);					\


#endif
