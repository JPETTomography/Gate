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
 *  @file AnalysisClassesManager.h
 */
#ifndef AnalysisClassesManager_h
#define AnalysisClassesManager_h

#include "../AnalysisClass/AnalysisClass.h"
#include <memory>
#include <string>
#include <map>

/**
 * @About class: This class is analysis classes manager - an singleton with all existing analysis classes
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class AnalysisClassesManager
{
 public:
  virtual ~AnalysisClassesManager();
  static AnalysisClassesManager* getInstance();

  AnalysisClass* getAnalysisClass( const std::string& analysis_class_name );
  void registerClass( const std::string& name, AnalysisClass* analysis_class );

 protected:
  AnalysisClassesManager();
  static std::unique_ptr<AnalysisClassesManager> upInstance;
  std::map< std::string, AnalysisClass* > mAnalysisClasses;
};

#endif
