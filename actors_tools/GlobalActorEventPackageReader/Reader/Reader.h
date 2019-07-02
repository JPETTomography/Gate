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
 *  @file Reader.h
 */
#ifndef Reader_h
#define Reader_h

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <string>

#include "../AnalysisClass/AnalysisClass.h"

#include "../Dictionaries/Dictionaries.h"

/**
 * @About class: This class reads data from Global Actor ROOT file, extract events from them and send them to analysis class
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class Reader
{
 public:
  Reader();
  ~Reader();
  bool startWorkFor( std::string file_name , AnalysisClass* analysis_class );

 private:
  bool openFile( std::string fileName );
  bool closeFile();
  bool readNextEvent();
  Dictionaries::Event getEvent() const;
  bool finishWithError( std::string msg ) const;

 private:
  TFile* pFile = nullptr;
  TTree* pTree = nullptr;
  int mNumberOfEvents = 0;
  int mCurrentEntryIndex = 0;
  const std::string mTreeName = "GateGlobalActorEventPackageTree";
  const std::string mBranchName = "GGAEvent";
  Dictionaries::Event* pEvent;
  TBranch* pBranch = nullptr;
};

#endif
