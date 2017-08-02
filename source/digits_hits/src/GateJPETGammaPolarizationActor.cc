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
 *  @file GateJPETGammaPolarizationActor.cc
 */

#include "G4VProcess.hh"
#include "GateJPETGammaPolarizationActor.hh"
#include "GateRunManager.hh"
#include "G4Run.hh"

#include "GateMiscFunctions.hh"
#include "GateObjectStore.hh"
#include "GateIAEAHeader.h"
#include "GateIAEARecord.h"
#include "GateIAEAUtilities.h"
#include "GateSourceMgr.hh"
#include "G4ParticleTable.hh"
#include "G4StepStatus.hh"

GateJPETGammaPolarizationActor::GateJPETGammaPolarizationActor(G4String name, G4int depth):GateVActor(name, depth)
{
	pMessenger = new GateJPETGammaPolarizationActorMessenger(this);
	mLogFileName = "";
	mDiagnosticFileName = "";
	mIsLogFileLoaded = false;
	mIsDiagnosticFileLoaded = false;
	mIsFirstStep=true;
	mEventID = -1;
	mComptonHappened = false;
	mAnglePrimeAndScatteredGammaPolarizationVectors = 0;
	mAnglePrimeAndScatteredGammaMomentumVectors = 0;
	pFile = 0;
	pListeVar = 0;
	mSkipThis = false;
	mPhi=0;
	mTheta=0;
	mSx=0;
	mSy=0;
	mSz=0;
	mUsePhiFilter = false;
	mUseThetaFilter = false;
	mPhiFilterLimes=0;
	mPhiFilterEpsilon=0;
	mThetaFilterLimes=0;
	mThetaFilterEpsilon=0;
}

GateJPETGammaPolarizationActor::~GateJPETGammaPolarizationActor()
{
	delete pMessenger;
	if(mIsLogFileLoaded)
		mLOG.close();
	if(mIsDiagnosticFileLoaded)
		mDGN.close();
}

void GateJPETGammaPolarizationActor::Construct()
{
	GateVActor::Construct();
	EnableBeginOfRunAction(true);
	EnableBeginOfEventAction(true);
	EnablePreUserTrackingAction(true);
	EnableUserSteppingAction(true);

	if(mIsLogFileLoaded)
		mLOG.open(mLogFileName.c_str(),std::ios::out);
	if(mIsDiagnosticFileLoaded)
		mDGN.open(mDiagnosticFileName.c_str(),std::ios::out);

	pFile = new TFile(mSaveFilename, "RECREATE", "ROOT file for phase space", 9);
	pListeVar = new TTree("JPETGamma", "JPET Gamma tree");
	pListeVar->Branch("Phi",&mPhi, "rad/D");
	pListeVar->Branch("Theta", &mTheta, "rad/D");
	pListeVar->Branch("Sx", &mSx, "rad/D");
	pListeVar->Branch("Sy", &mSy, "rad/D");
	pListeVar->Branch("Sz", &mSz, "rad/D");

}

void GateJPETGammaPolarizationActor::PreUserTrackingAction(const GateVVolume* /*v*/, const G4Track* /*t*/)
{
	mIsFirstStep = true;
}

void GateJPETGammaPolarizationActor::BeginOfEventAction(const G4Event* /*e*/)
{

}

void GateJPETGammaPolarizationActor::UserSteppingAction(const GateVVolume*, const G4Step *step)
{
	if(step->GetTrack()->GetDefinition()->GetParticleName() == "gamma")
		StandardExtractFunction(step);
}

void GateJPETGammaPolarizationActor::SetLogFile(G4String log_file_name)
{
	if(log_file_name.size()>0){
		mLogFileName = log_file_name;
		mIsLogFileLoaded = true;
	}else{
		mIsLogFileLoaded = false;
	}
}

void GateJPETGammaPolarizationActor::SetDiagnosticFile(G4String diag_file_name)
{
	if(diag_file_name.size()>0){
		mDiagnosticFileName = diag_file_name;
		mIsDiagnosticFileLoaded = true;
	}else{
		mIsDiagnosticFileLoaded = false;
	}
}


void GateJPETGammaPolarizationActor::SetPhiFlagParameter(double value, GateJPETGammaPolarizationActor::PhiThetaValueMode mode)
{
	switch(mode){
		case GateJPETGammaPolarizationActor::Flag:{
			mUsePhiFilter =  value >0;
			break;
		}
		case GateJPETGammaPolarizationActor::Limes:{
			mPhiFilterLimes = value;
			break;
		}
		case GateJPETGammaPolarizationActor::Epsilon:{

			if(value >0){
				mPhiFilterEpsilon  = value;
			}else{
				mUsePhiFilter = false;
			}

			break;
		}
	}
}

void GateJPETGammaPolarizationActor::SetThetaFlagParameter(double value, GateJPETGammaPolarizationActor::PhiThetaValueMode mode)
{
	switch(mode){
		case GateJPETGammaPolarizationActor::Flag:{
			mUseThetaFilter =  value >0;
			break;
		}
		case GateJPETGammaPolarizationActor::Limes:{
			mThetaFilterLimes = value;
			break;
		}
		case GateJPETGammaPolarizationActor::Epsilon:{

			if(value >0){
				mThetaFilterEpsilon  = value;
			}else{
				mUseThetaFilter = false;
			}

			break;
		}
	}
}

void GateJPETGammaPolarizationActor::StandardExtractFunction(const G4Step *step)
{
	if(mEventID != GateRunManager::GetRunManager()->GetCurrentEvent()->GetEventID()){
		mEventID = GateRunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
		mComptonHappened = false;

		if(step->GetPreStepPoint()->GetStepStatus()== G4StepStatus::fGeomBoundary){
			mPrimePolarization = step->GetPreStepPoint()->GetPolarization();
			mSkipThis = false;
		}else{
			mSkipThis = true;
		}
	}

	if(mSkipThis)
		return;

	if(mComptonHappened)
		return;

	if(!(step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName()=="Compton"))
		return;

	mComptonHappened = true;

	//Gamma momentum direction after emission
	G4ThreeVector gamma_momentum_direction_after_emission = step->GetTrack()->GetVertexMomentumDirection();
	//Gamma momentum direction after Compton
	G4ThreeVector gamma_momentum_direction_after_compton = step->GetPreStepPoint()->GetMomentumDirection();

	double phi =0, theta =0;
	//Convert angle from radians to degrees
	phi = 180*gamma_momentum_direction_after_compton.phi()/M_PI;
	if(mAllPhiCalcNoNegative && phi<0){
		phi = phi + 180;
	}

	theta = 180*gamma_momentum_direction_after_compton.theta()/M_PI;

	mPhi = phi;
	mTheta = theta;

	//It's this idea that cross product is prime gamma polarization (it's approximation)
	G4ThreeVector gamma_reconstructed_polarization = gamma_momentum_direction_after_emission.cross(gamma_momentum_direction_after_compton);
	//Rotate twice - sign must be the same
	(gamma_reconstructed_polarization.rotateUz(gamma_momentum_direction_after_emission)).rotateUz(gamma_momentum_direction_after_emission);
	mSx = gamma_reconstructed_polarization.x();
	mSy = gamma_reconstructed_polarization.y();
	mSz = gamma_reconstructed_polarization.z();

	//Save to file
	pListeVar->Fill();
	return;

}

void GateJPETGammaPolarizationActor::SetAllPhiAsNoNegative(bool setAsPlus)
{
	mAllPhiCalcNoNegative = setAsPlus;
}

void GateJPETGammaPolarizationActor::SaveData()
{
	GateVActor::SaveData();
	pFile = pListeVar->GetCurrentFile();
	pFile->Write();
}

void GateJPETGammaPolarizationActor::ResetData() {
	pListeVar->Reset();
}
