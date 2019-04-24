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
 *  @file Reader.cpp
 */
#include "Reader.h"
#include <iostream>

Reader::Reader() {}

Reader::~Reader() {}

bool Reader::startWorkFor( std::string file_name , AnalysisClass* analysis_class )
{
 if ( analysis_class == nullptr ) { return finishWithError( "Analysis class null pointer" ); }
 if ( !openFile( file_name ) ) { return false; }
 while ( readNextEvent() ) { analysis_class->noticeEvent( getEvent() ); }
 return closeFile();
}

bool Reader::openFile( std::string fileName )
{
 if ( fileName.size() == 0 ) { return finishWithError( "File name not provided" ); }
 if ( fileName.rfind(".root") == std::string::npos) { fileName += ".root"; }
 pFile = new TFile( fileName.c_str(), "READ" );
 if ( !pFile->IsOpen() ) { return finishWithError( "Opening file " + fileName + " failed" ); }
 pTree = dynamic_cast<TTree*>( pFile->Get( mTreeName.c_str() ) );
 if ( !pTree ) { return finishWithError( "Loading tree failed" ); }
 mNumberOfEvents = pTree->GetEntries();
 if ( mNumberOfEvents == 0 ) { return finishWithError( "File is empty" ); }
 if ( pTree->FindBranch( mBranchName.c_str() ) == nullptr ) { return finishWithError( "Loading branch with events failed" ); }
 pTree->SetBranchAddress( mBranchName.c_str(), &pEvent, &pBranch );
 return true;
}

bool Reader::closeFile()
{
 assert( pFile != nullptr );
 pFile->Close();
 return true;
}

bool Reader::readNextEvent()
{
 if ( mCurrentEntryIndex < mNumberOfEvents )
 { 
  pTree->GetEntry( mCurrentEntryIndex); 
  ++mCurrentEntryIndex;
  return true;
 }
 return false;
}

Dictionaries::Event Reader::getEvent() const
{
 return *pEvent;
}

bool Reader::finishWithError( std::string msg ) const
{
 std::cerr << "[ERROR] " << msg << std::endl;
 return false;
}
