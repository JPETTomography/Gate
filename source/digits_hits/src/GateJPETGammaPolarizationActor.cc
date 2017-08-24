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
#include <cmath>
#include "Randomize.hh"
#include "TH2.h"
#include "TF1.h"


GateJPETGammaPolarizationActor::GateJPETGammaPolarizationActor(G4String name, G4int depth) : GateVActor(name, depth)
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
	mPhi=0;
	mTheta=0;
	mUsePhiFilter = false;
	mUseThetaFilter = false;
	mPhiFilterLimes=0;
	mPhiFilterEpsilon=0;
	mThetaFilterLimes=0;
	mThetaFilterEpsilon=0;
	mAngularAccuracy = 2;
	mSaveHistograms = true;
	mSaveTests = true;
}

GateJPETGammaPolarizationActor::~GateJPETGammaPolarizationActor()
{
	delete pMessenger;
	if(mIsLogFileLoaded)
		mLOG.close();
	if(mIsDiagnosticFileLoaded)
		mDGN.close();
	if(mSaveHistograms)
	{
		delete pCrossSectionOfPhi;
		delete pCrossSectionOfTheta;
		delete pRealCrossSectionOfTheta;
	}
	delete pFile;
	delete pListeVar;
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

	pFile = new TFile(mSaveFilename, "RECREATE", "ROOT file of polarized and unpolarized gamma compton scattering", 9);

	//Histograms

	if(mSaveHistograms)
	{
		double control_bins_number = 0;
		int bins_number = 0;

		//Hstogram of phi cross-section

		if(mAllPhiCalcNoNegative)
			control_bins_number = 360.0/mAngularAccuracy;
		else
			control_bins_number = 180.0/mAngularAccuracy;

		bins_number = (int)control_bins_number;
		pCrossSectionOfPhi = new TH1F("csPhi", "", bins_number, mAllPhiCalcNoNegative ? 0.0 : -180.0, 180.0);

		pCrossSectionOfPhi->GetXaxis()->SetTitle("#phi [deg]");
		pCrossSectionOfPhi->GetYaxis()->SetTitle("d#sigma/d#Omega [a.u.]");

		//Hstogram of theta cross-section

		control_bins_number = 180.0/mAngularAccuracy;
		bins_number = (int)control_bins_number;
		pCrossSectionOfTheta = new TH1F("csTheta", "", bins_number,  0.0, 180.0);

		pCrossSectionOfTheta->GetXaxis()->SetTitle("#theta [deg]");
		pCrossSectionOfTheta->GetYaxis()->SetTitle("d#sigma/d#theta [a.u.]");

		pRealCrossSectionOfTheta  = new TH1F("rcsTheta", "", bins_number,  0.0, 180.0);

		pRealCrossSectionOfTheta->GetXaxis()->SetTitle("#theta [deg]");
		pRealCrossSectionOfTheta->GetYaxis()->SetTitle("d#sigma/d#Omega [a.u.]");
	}

	pListeVar = new TTree("JPETGammaPolarizationAnalysis", "JPET Gamma polarization analysis tree");
	pListeVar->Branch("Phi",&mPhi, "rad/D");
	pListeVar->Branch("Theta", &mTheta, "rad/D");
	if(mSaveTests)
		pListeVar->Branch("Tests",&mTests, "SGPA:GA");

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
	if (log_file_name.size() > 0)
	{
		mLogFileName = log_file_name;
		mIsLogFileLoaded = true;
	}
	else
	{
		mIsLogFileLoaded = false;
	}
}

void GateJPETGammaPolarizationActor::SetDiagnosticFile(G4String diag_file_name)
{
	if(diag_file_name.size() > 0)
	{
		mDiagnosticFileName = diag_file_name;
		mIsDiagnosticFileLoaded = true;
	}
	else
	{
		mIsDiagnosticFileLoaded = false;
	}
}


void GateJPETGammaPolarizationActor::SetPhiFlagParameter(double value, GateJPETGammaPolarizationActor::PhiThetaValueMode mode)
{
	switch(mode)
	{
		case GateJPETGammaPolarizationActor::Flag:
		{
			mUsePhiFilter =  value > 0;
			break;
		}
		case GateJPETGammaPolarizationActor::Limes:
		{
			mPhiFilterLimes = value;
			break;
		}
		case GateJPETGammaPolarizationActor::Epsilon:
		{
			if(value > 0)
				mPhiFilterEpsilon  = value;
			else
				mUsePhiFilter = false;
			break;
		}
	}
}

void GateJPETGammaPolarizationActor::SetThetaFlagParameter(double value, GateJPETGammaPolarizationActor::PhiThetaValueMode mode)
{
	switch(mode)
	{
		case GateJPETGammaPolarizationActor::Flag:
		{
			mUseThetaFilter =  value >0;
			break;
		}
		case GateJPETGammaPolarizationActor::Limes:
		{
			mThetaFilterLimes = value;
			break;
		}
		case GateJPETGammaPolarizationActor::Epsilon:
		{
			if(value > 0)
				mThetaFilterEpsilon  = value;
			else
				mUseThetaFilter = false;
			break;
		}
	}
}

void GateJPETGammaPolarizationActor::StandardExtractFunction(const G4Step *step)
{
	if(mEventID != GateRunManager::GetRunManager()->GetCurrentEvent()->GetEventID())
	{
		mEventID = GateRunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
		mComptonHappened = false;
		mPrimeGammaPolarization = step->GetPreStepPoint()->GetPolarization().unit();
		mPrimeGammaDirection = step->GetPreStepPoint()->GetMomentumDirection().unit();
	}
	else if(mComptonHappened)
	{
		return;
	}

	//So it is first function call or next but Compton's scatter didn't happen in post step
	if(!(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="Compton"))
			return;

	//We caught first Compton's scatter
	mComptonHappened = true;

	/** This is most important part of actor.
	 *  This is based on article "New Monte Carlo method for Compton and Rayleigh scattering by polarized gamma rays ,G.O. Depaola" and structure of function G4LivermorePolarizedComptonModel::SystemOfRefChange.
	 *  k0 - prime gamma momentum direction vector
	 *  k  - gamma momentum direction vector after scatter
	 *  k_xy - projection of k vector on x-y plane
	 *  e0 - prime gamma polarization vector
	 *  We calculate phi and theta in local Cartesian system defined as:
	 *  AxisX = e0
	 *  AxisY = AxisZ x AxisX = k0 x e0, where "x" is cross product
	 *  AxisZ = k0
	 *  and in this assumptions :
	 *  theta is angle between k0 and k vectors
	 *  phi is angle between e0 and k_xy vectors
	 *
	 *  As you can see polarization of prime gamma play only role of coordinate system and has not influence on scattered gamma.
	 * */

	G4ThreeVector k0,k,k_xy,e0,e;
	double phi =0, theta =0;
	k0 = mPrimeGammaDirection;
	e0 = mPrimeGammaPolarization;
	k = step->GetPostStepPoint()->GetMomentumDirection().unit();
	e = step->GetPostStepPoint()->GetPolarization().unit();
	theta =  k0.angle(k);

	if(e0.mag() == 0)
	{
		//In case when prime gamma is unpolarized we can't say anything about prime polarization
		//But we know that in this case K-N cross-section for phi must be uniform - that why we generate here random phi
		phi = 2*M_PI*G4UniformRand()-M_PI;
	}
	else
	{
		k_xy = k*sin(theta);
		phi = e0.angle(k_xy);
	}

	theta *= 180.0/M_PI;
	phi *= 180.0/M_PI;

	if(mAllPhiCalcNoNegative && phi < 0)
		phi = phi + 180;

	if(mUsePhiFilter)
	{
		if((mPhiFilterLimes - phi)*(mPhiFilterLimes - phi) > mPhiFilterEpsilon*mPhiFilterEpsilon)
			return;
	}

	if(mUseThetaFilter)
	{
		if((mThetaFilterLimes - theta)*(mThetaFilterLimes - theta) > mThetaFilterEpsilon*mThetaFilterEpsilon)
			return;
	}

	mPhi = phi;
	mTheta = theta;

	//If prime gamma is unpolarized below section has no sense
	if(mSaveTests && e0.mag() != 0){
		/** mScatteredGammaPolarizationAngle inform what was a new polarization after Compton's scatter.
		 * */
		double dzeta = e0.angle(k);
		double huge_theta = e0.angle(e);
		double sin_dzeta,cos_huge_theta;
		sin_dzeta = sin(dzeta);
		cos_huge_theta = cos(huge_theta);
		double val = std::abs(sin_dzeta) <= pow(10,-6) ? 0.0 : cos_huge_theta/sin_dzeta;
		val = abs(val) == 1 ? 1.0 : val;
		double polarization_angle = std::abs(sin_dzeta) <= pow(10,-6) ? 0.0 : std::acos(val);
		polarization_angle *= 180.0/M_PI;
		mTests.ScatteredGammaPolarizationAngle = polarization_angle;

		/** mAngularDiffreneceBetweenRealAndReconstructedPrimeGammaPolarization inform about difference between real linear polarization angle (from simulation)
		 * and polarization reconstructed as cross product of prime and scattered gamma momentum direction.
		 * */
		double angular_diffrence_between_real_and_reconstructed_polarization = e0.angle(k0.cross(k).unit());
		angular_diffrence_between_real_and_reconstructed_polarization *= 180.0/M_PI;
		mTests.AngularDiffreneceBetweenRealAndReconstructedPrimeGammaPolarization = angular_diffrence_between_real_and_reconstructed_polarization;
	}

	if(mSaveHistograms)
	{
		pCrossSectionOfPhi->Fill(mPhi,1);
		pCrossSectionOfTheta->Fill(mTheta,1);
		pRealCrossSectionOfTheta->Fill(mTheta,1);
	}
	pListeVar->Fill();
}

void GateJPETGammaPolarizationActor::SetAllPhiAsNoNegative(bool setAsPlus)
{
	mAllPhiCalcNoNegative = setAsPlus;
}

void GateJPETGammaPolarizationActor::SaveData()
{
	GenerateRealCrossSectionOfTheta();
	NormalizeHistograms();
	GateVActor::SaveData();
	pFile = pListeVar->GetCurrentFile();
	pFile->Write();
}

void GateJPETGammaPolarizationActor::ResetData()
{
	pListeVar->Reset();
}

G4ThreeVector GateJPETGammaPolarizationActor::SetPerpendicularVector(G4ThreeVector& a)
{
  G4double dx = a.x();
  G4double dy = a.y();
  G4double dz = a.z();
  G4double x = dx < 0.0 ? -dx : dx;
  G4double y = dy < 0.0 ? -dy : dy;
  G4double z = dz < 0.0 ? -dz : dz;
  if (x < y) {
    return x < z ? G4ThreeVector(-dy,dx,0) : G4ThreeVector(0,-dz,dy);
  }else{
    return y < z ? G4ThreeVector(dz,0,-dx) : G4ThreeVector(-dy,dx,0);
  }
}

void GateJPETGammaPolarizationActor::GenerateRealCrossSectionOfTheta()
{
	if(!mSaveHistograms)
		return;

	int counter = pRealCrossSectionOfTheta->Integral();
	TF1 func("func", "sin(TMath::DegToRad()*x)", 0.0, 180.0);
	double control_bins_number = 180.0/mAngularAccuracy;
	int bins_number = (int)control_bins_number;
	TH1F sinus_histogram("sinus_histogram","",bins_number,0,180.0);

	for(int i = 1; i <= counter; ++i)
		sinus_histogram.Fill(func.GetRandom());

	pRealCrossSectionOfTheta->Divide(&sinus_histogram);
}

void GateJPETGammaPolarizationActor::NormalizeHistograms()
{
	if(!mSaveHistograms)
			return;

	pCrossSectionOfPhi->Scale(pow(pCrossSectionOfPhi->Integral(),-1.0));
	pCrossSectionOfTheta->Scale(pow(pCrossSectionOfTheta->Integral(),-1.0));
	pRealCrossSectionOfTheta->Scale(pow(pRealCrossSectionOfTheta->Integral(),-1.0));
}

void GateJPETGammaPolarizationActor::SetHistogramsEnable(bool enable)
{
	mSaveHistograms = enable;
}

void GateJPETGammaPolarizationActor::SetTestsBranchEnable(bool enable)
{
	mSaveTests = enable;
}

void GateJPETGammaPolarizationActor::SetAnglePrecisionPerBinInHistograms(double angle_precision)
{
	if(angle_precision >= 180.0 || angle_precision <= 0)
	{
		G4cout<<"[WARRNING] Invalide value for angle precision in histograms from class GateJPETGammaPolarizationActor - this value will be neglected. Allowed range of values : more then 0 and less then 180 degree. "<<G4endl;
		return;
	}
	mAngularAccuracy = angle_precision;
}
