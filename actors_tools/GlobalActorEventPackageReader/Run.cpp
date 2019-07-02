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
 *  @file Run.cpp
 */
#include <iostream>
#include <string>
#include "AnalysisClassesManager/AnalysisClassesManager.h"
#include "Reader/Reader.h"

int returnError( int n, std::string msg ) 
{
 std::cerr << "[ERROR] " << msg << std::endl;
 std::cerr << "Return code " << n << std::endl;
 return n;
}

int main( int argc, char* argv[] )
{

 if ( argc < 3 )
 {
  std::cerr << "[ERROR] Not provided required number of parameters. Try ./reader_gaep data_file.root analysis_class_name [other parameters for analysis class ]" << std::endl;
  return returnError( 1, "Parameter error" );
 }

 std::string file_name( argv[1] );
 std::string analysis_name( argv[2] );

 AnalysisClass* analysis_class = AnalysisClassesManager::getInstance()->getAnalysisClass( analysis_name );

 if ( analysis_class == nullptr ) { return returnError( 2, "Analysis class not found" ); }
 analysis_class->readAdditionalArguments( argc, argv );

 Reader r;
 if ( !r.startWorkFor( file_name, analysis_class ) ) { return returnError( 3, "Reader failed" ); }
 analysis_class->saveData();
 analysis_class->displayStatus();
 return 0;

}
