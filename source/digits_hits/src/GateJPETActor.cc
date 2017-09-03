/**
 *  @copyright Copyright 2017 The J-PET Gate Authors. All rights reserved.
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
 *  @file GateJPETActor.cc
 */

#include "GateJPETActor.hh"
#include "G4VProcess.hh"
#include "GateRunManager.hh"
#include "G4Run.hh"

#include "GateMiscFunctions.hh"
#include "GateObjectStore.hh"
#include "GateIAEAHeader.h"
#include "GateIAEARecord.h"
#include "GateIAEAUtilities.h"
#include "GateSourceMgr.hh"
#include "G4ParticleTable.hh"

GateJPETActor::GateJPETActor(G4String name, G4int depth):GateVActor(name, depth)
{
	GateDebugMessageInc("Actor", 4, "GateJPETActor() -- begin" << G4endl);
	pMessenger = new GateJPETActorMessenger(this);
	pFile = nullptr;
	pListeVar = nullptr;

	//For default set everything for FALSE
	 mEnableXPosition = false;
	 mEnableYPosition= false;
	 mEnableZPosition= false;
	 mEnableParticleName= false;
	 mEnableTrackID= false;
	 mEnableEventID= false;
	 mEnableParentID= false;
	 mEnableRunID= false;
	 mEnableKineticEnergy= false;
	 mEnableInteractionTime= false;
	 mEnableProcessName= false;
	 mEnableEmissionPoint = false;

	 mFileType = " ";

	 mIsFirstStep=true;

	 GateDebugMessageDec("Actor", 4, "GateJPETActor() -- end" << G4endl);
}

GateJPETActor::~GateJPETActor()
{
	if(!mASCIFileName.empty())
		mASCIFile.close();
	delete pFile;
	delete pListeVar;
	delete pMessenger;

}

void GateJPETActor::Construct()
{
	GateVActor::Construct();
	EnableBeginOfRunAction(false);
	EnableBeginOfEventAction(false);
	EnablePreUserTrackingAction(true);
	EnableUserSteppingAction(true);
	G4String extension = getExtension(mSaveFilename);

	G4cout <<"root file name "<<mSaveFilename<< G4endl;
	G4cout <<"exten: "<< extension<<G4endl;
	//Yup -it's only for root files :P
	if(extension == "root"){
		mFileType = "rootFile";
	}else{
		GateError( "Unknow phase space file extension. Knowns extensions are .root"<< G4endl);
	}

	//Here is extra for ASCI data
	if(!mASCIFileName.empty()){
		mASCIFile.open(mASCIFileName.c_str(), std::ios::out);
	}

	//OK - so what we want to save?
	if (mFileType == "rootFile"){
		pFile = new TFile(mSaveFilename, "RECREATE", "ROOT file for phase space", 9);
		pListeVar = new TTree("JPET", "JPET tree");

		if(mEnableXPosition)
			pListeVar->Branch("ParticleXPosition", &mPositionX, "ParticleXPosition/D");
		if(mEnableYPosition)
			pListeVar->Branch("ParticleYPosition", &mPositionY, "ParticleYPosition/D");
		if(mEnableZPosition)
			pListeVar->Branch("ParticleZPosition", &mPositionZ, "ParticleZPosition/D");
		if(mEnableParticleName)
			pListeVar->Branch("ParticleName", &mParticleName , "ParticleName/C");
		if(mEnableTrackID)
			pListeVar->Branch("TrackID", &mTrackID, "TrackID/I");
		if(mEnableEventID)
			pListeVar->Branch("EventID", &mEventID, "EventID/I");
		if(mEnableParentID)
			pListeVar->Branch("ParentID", &mParentID, "ParentID/I");
		if(mEnableRunID)
			pListeVar->Branch("RunID", &mRunID, "RunID/I");
		if(mEnableKineticEnergy)
			pListeVar->Branch("ParticleKineticEnergy", &mKinecticEnergy, "ParticleKineticEnergy/D");
		if(mEnableInteractionTime)
			pListeVar->Branch("InteractionsTime", &mInteractionTime, "InteractionsTime/D");
		if(mEnableProcessName)
			pListeVar->Branch("ProcessName", &mProcessName , "ProcessName/C");
		if(mEnableEmissionPoint){
			pListeVar->Branch("EmissionPointX", &mEmissionPositionX, "EmissionPointX/D");
			pListeVar->Branch("EmissionPointY", &mEmissionPositionY, "EmissionPointY/D");
			pListeVar->Branch("EmissionPointZ", &mEmissionPositionZ, "EmissionPointZ/D");
		}
		if(mEnablePrimaryEnergy)
			pListeVar->Branch("ParticlePrimaryEnergy", &mPrimaryEnergy, "ParticlePrimaryEnergy/D");

	}
}

void GateJPETActor::PreUserTrackingAction(const GateVVolume * /*v*/, const G4Track * t)
{
	mIsFirstStep = true;
	if(mEnableEmissionPoint){
		mEmissionPositionX = t->GetVertexPosition().x();
		mEmissionPositionY = t->GetVertexPosition().y();
		mEmissionPositionZ = t->GetVertexPosition().z();
	}
}

void GateJPETActor::BeginOfEventAction(const G4Event *e)
{
	if(mEnablePrimaryEnergy)
		mPrimaryEnergy = e->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
}

void GateJPETActor::UserSteppingAction(const GateVVolume*, const G4Step *step)
{
	if(mParticleNames.size()==0){
		StandardExtractFunction( step);
	}else{
		std::vector<G4String>::iterator found = std::find(mParticleNames.begin(),mParticleNames.end(),step->GetTrack()->GetDefinition()->GetParticleName());
		if(found != mParticleNames.end()){
			StandardExtractFunction(step);
		}
	}
	 mIsFirstStep = false;
}

void GateJPETActor::StandardExtractFunction(const G4Step *step)
{
	//First save ASCI data
	if(!mASCIFileName.empty()){
		G4ThreeVector localPosition = step->GetPostStepPoint()->GetPosition();
		mASCIFile<<step->GetTrack()->GetDefinition()->GetParticleName()<<" "<<localPosition.x()<<" "<<localPosition.y()<<" "<<localPosition.z()<<"\n";
	}


	if(mEnableXPosition || mEnableYPosition || mEnableZPosition){
		G4ThreeVector localPosition = step->GetPostStepPoint()->GetPosition();
		if(mEnableXPosition)
			mPositionX  = localPosition.x();
		if(mEnableYPosition)
			mPositionY = localPosition.y();
		if(mEnableZPosition)
			mPositionZ = localPosition.z();
	}

	if(mEnableParticleName){
		G4String str = step->GetTrack()->GetDefinition()->GetParticleName();
		strcpy(mParticleName, str.c_str());
	}

	if(mEnableTrackID)
		mTrackID = step->GetTrack()->GetTrackID();
	if(mEnableEventID)
		mEventID = GateRunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	if(mEnableRunID)
		mRunID = GateRunManager::GetRunManager()->GetCurrentRun()->GetRunID();
	if(mEnableParentID)
		mParentID = step->GetTrack()->GetParentID();
	if(mEnableKineticEnergy)
		mKinecticEnergy = step->GetPostStepPoint()->GetKineticEnergy();
	if(mEnableInteractionTime)
		mInteractionTime = step->GetPostStepPoint()->GetLocalTime();
	if(mEnableProcessName) {
		G4String str = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
		strcpy(mProcessName, str.c_str());
	}

	pListeVar->Fill();
}

void GateJPETActor::SaveData()
{
	GateVActor::SaveData();
	if (mFileType == "rootFile") {
		pFile = pListeVar->GetCurrentFile();
		pFile->Write();
	}
}

void GateJPETActor::ResetData() {
  if (mFileType == "rootFile") {
    pListeVar->Reset();
    return;
  }
}

void GateJPETActor::SetXPositionEnabled(bool enableXPosition)
{
	mEnableXPosition = enableXPosition;
}


void GateJPETActor::SetYPositionEnabled(bool enableYPosition)
{
	mEnableYPosition = enableYPosition;
}

void GateJPETActor::SetZPositionEnabled(bool enableZPosition)
{
	mEnableZPosition = enableZPosition;
}

void GateJPETActor::SetParticleNameEnabled(bool enableParticleName)
{
	mEnableParticleName = enableParticleName;
}

void GateJPETActor::SetParticleEnabled(G4String particleName)
{
	mParticleNames.push_back(particleName);
}

void GateJPETActor::SetTrackIDEnabled(bool enableTrackID)
{
	mEnableTrackID = enableTrackID;
}


void GateJPETActor::SetEventIDEnabled(bool enableEventID)
{
	mEnableEventID = enableEventID;
}

void GateJPETActor::SetParentIDEnabled(bool enableParentID)
{
	mEnableParentID = enableParentID;
}

void GateJPETActor::SetRunIDEnabled(bool enableRunID)
{
	mEnableRunID = enableRunID;
}

void GateJPETActor::SetEkinEnabled(bool enableEkin)
{
	mEnableKineticEnergy = enableEkin;
}

void GateJPETActor::SetInteractionTimeEnabled(bool enableInteractionTime)
{
	mEnableInteractionTime = enableInteractionTime;
}

void GateJPETActor::SetProcessNameEnabled(bool enableProcessName)
{
	mEnableProcessName = enableProcessName;
}

void GateJPETActor::SetEmissionPointEnabled(bool enableEmissionPoint)
{
	mEnableEmissionPoint = enableEmissionPoint;
}
void GateJPETActor::SetPrimaryEnergy(bool enablePrimaryEnergy)
{
	mEnablePrimaryEnergy = enablePrimaryEnergy;
}

void GateJPETActor::SetASCIFileName(std::string fileName)
{
	mASCIFileName = fileName;
}
