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
 *  @file AnalysisClassesManager.cpp
 */
#include<iostream>
#include "AnalysisClassesManager.h"

std::unique_ptr<AnalysisClassesManager> AnalysisClassesManager::upInstance;

AnalysisClassesManager::AnalysisClassesManager() { }

AnalysisClassesManager::~AnalysisClassesManager()
{
 for ( std::map< std::string, AnalysisClass* >::iterator it = mAnalysisClasses.begin(); it != mAnalysisClasses.end(); ++it ) { delete it->second; }
 mAnalysisClasses.clear();
}

AnalysisClassesManager* AnalysisClassesManager::getInstance()
{
 if( upInstance.get() == nullptr ) { upInstance.reset( new AnalysisClassesManager() ); }
 return upInstance.get();
}

AnalysisClass* AnalysisClassesManager::getAnalysisClass( std::string analysis_class_name )
{
 std::map< std::string, AnalysisClass* >::iterator found = mAnalysisClasses.find( analysis_class_name );
 if ( found == mAnalysisClasses.end() ) { return nullptr; }
 return found->second;
}


void AnalysisClassesManager::registerClass( std::string name, AnalysisClass* analysis_class )
{
 mAnalysisClasses.emplace( name, analysis_class );
}
