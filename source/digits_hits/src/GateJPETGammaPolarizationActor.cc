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
#include<string>


GateJPETGammaPolarizationActor::GateJPETGammaPolarizationActor(G4String name, G4int depth) : GateVActor(name, depth)
{
	pMessenger = new GateJPETGammaPolarizationActorMessenger(this);
	pFile = nullptr;
	pListeVar = nullptr;
	pCrossSectionOfPhi = nullptr;
	pCrossSectionOfTheta = nullptr;
	pRealCrossSectionOfTheta = nullptr;
	pAllParticlesReacted = nullptr;
	mLogFileName = "";
	mDiagnosticFileName = "";
	mIsLogFileLoaded = false;
	mIsDiagnosticFileLoaded = false;
	mEventID = -1;
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
	mSaveOnlyWhenTheDesiredNumberOfParticlesHasScatteredFromEvent = false;
	mDesiredNumberOfParticlesScatteredPerEvent = 0;
	mScatteredParticlesPerEvent = 0;
	mAllPhiCalcNoNegative = false;
	mPrimeEnergy = 0;
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
		delete pAllParticlesReacted;
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

		if(!mAllPhiCalcNoNegative)
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

		pAllParticlesReacted = new TH1F("prcs", "", mDesiredNumberOfParticlesScatteredPerEvent+2,  0, mDesiredNumberOfParticlesScatteredPerEvent+1);
		pAllParticlesReacted->GetXaxis()->SetTitle("Particles number reactions");
		pAllParticlesReacted->GetYaxis()->SetTitle("Events number");
	}

	pListeVar = new TTree("JPETGammaPolarizationAnalysis", "JPET Gamma polarization analysis tree");
	pListeVar->Branch("Phi",&mPhi, "rad/D");
	pListeVar->Branch("Theta", &mTheta, "rad/D");
	pListeVar->Branch("PrimeEnergy", &mPrimeEnergy, "MeV/D");
	if(mSaveTests)
		pListeVar->Branch("Tests",&mTests, "SGPA:GA");

	DisplaySummarizeBeforeRun();
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
	bool skip_this_step = false;
	G4int track_id = step->GetTrack()->GetTrackID();

	if(mEventID != GateRunManager::GetRunManager()->GetCurrentEvent()->GetEventID())
	{
		if(mSaveHistograms && mEventID >= 0)
		{
			if(mScatteredParticlesPerEvent < pAllParticlesReacted->GetXaxis()->GetXmax())
				pAllParticlesReacted->Fill(mScatteredParticlesPerEvent,1);
		}

		mScatteredParticlesPerEvent = 0;
		mEventID = GateRunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
		mEventTracks.clear();
		AddETP(step);
	}
	else
	{
		 std::map<G4int, EventTrackPartner>::iterator found_track =  mEventTracks.find(track_id);
		 if(found_track != mEventTracks.end())
		 {
			 if(found_track->second.ComptonHappened)
				 skip_this_step = true;
		 }
		 else
		 {
			 AddETP(step);
		 }
	}

	if(skip_this_step)
		return;

	//So it is first function call or next but Compton's scatter didn't happen in post step
	if(!(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="Compton"))
		return;

	//We caught first Compton's scatter
	mEventTracks[track_id].ComptonHappened = true;
	++mScatteredParticlesPerEvent;

	G4ThreeVector k0,k,e0,e;
	k0 = mEventTracks[track_id].PrimeGammaDirection;
	e0 = mEventTracks[track_id].PrimeGammaPolarization;
	k = step->GetPostStepPoint()->GetMomentumDirection().unit();
	e = step->GetPostStepPoint()->GetPolarization().unit();

	GetThetaAndPhi(k0, k, e0, mEventTracks[track_id].ComptonTheta, mEventTracks[track_id].ComptonPhi);
	mEventTracks[track_id].ComptonInteractionPoint = step->GetPostStepPoint()->GetPosition();
	mEventTracks[track_id].ComptonTotatEnergy = step->GetPostStepPoint()->GetTotalEnergy();
	mEventTracks[track_id].Tests.ScatteredGammaPolarizationAngle = GetPolaizationAngle(k,e0,e);
	mEventTracks[track_id].Tests.AngularDiffreneceBetweenRealAndReconstructedPrimeGammaPolarization = GetAngleDifference(k,k0,e0);

	if(mSaveOnlyWhenTheDesiredNumberOfParticlesHasScatteredFromEvent)
	{
		if(mScatteredParticlesPerEvent == mDesiredNumberOfParticlesScatteredPerEvent)
			for( std::map<G4int, EventTrackPartner>::iterator it = mEventTracks.begin(); it != mEventTracks.end(); ++it)
				if(it->second.ComptonHappened)
					FillWithData(mEventID, it->first, &it->second);
	}
	else
	{
		FillWithData(mEventID,track_id,&mEventTracks[track_id]);
	}
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

G4ThreeVector GateJPETGammaPolarizationActor::SetPerpendicularVector(const G4ThreeVector& a)
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

void GateJPETGammaPolarizationActor::SaveToFile(const G4int& eventID, const G4int& trackID, const G4double& phi, const G4double& theta, const G4double& polarization, const G4ThreeVector& interactionPoint, const G4double& totalEnergy, const G4ThreeVector& emisionPoint, const G4double& primeEnergy, const G4double& emisionPhi, const G4double& emisionTheta)
{
	mDGN<<eventID<<" "<<trackID<<" "<<phi<<" "<<theta<<" "<<polarization<<" "<<interactionPoint.x()<<" "<<interactionPoint.y()<<" "<<interactionPoint.z()<<" "<<totalEnergy<<" "<<emisionPoint.x()<<" "<<emisionPoint.y()<<" "<<emisionPoint.z()<<" "<<primeEnergy<<" "<<emisionPhi<<" "<<emisionTheta<<std::endl;
}

void GateJPETGammaPolarizationActor::GetThetaAndPhi(const G4ThreeVector& k0, const G4ThreeVector& k, const G4ThreeVector& e0, G4double& theta, G4double& phi)
{
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
	*  phi is definited as: k*e0 = sin(theta)*cos(phi), where |k|=|e0| = 1 (based on "New Monte Carlo method for Compton and Rayleigh scattering by polarized gamma rays", G.O. Depaola, Nuclear Instruments and Methods in Physics Research A 512 (2003) 619–630)
	*
	*  As you can see polarization of prime gamma play only role of coordinate system and has not influence on scattered gamma.
	* */
	theta =  k0.angle(k);

	if(e0.mag() == 0)
	{
		//In case when prime gamma is unpolarized we can't say anything about prime polarization
		//But we know that in this case K-N cross-section for phi must be uniform - that why we generate here random phi
		phi = M_PI*(2.0*G4UniformRand()-1.0);
	}
	else
	{
		G4ThreeVector versorY = k0.cross(e0).unit();
		G4ThreeVector versorX = e0;

		G4double k_x = k.dot(versorX);
		G4double k_y = k.dot(versorY);
		G4double sinTheta = sin(theta);
		if(sinTheta == 0){
			/** Please look to an article "New Monte Carlo method for Compton and Rayleigh scattering by polarized gamma rays" and study a Fig.1.a;
			 * In a brief:
			 * Tha angle theta is an angle between a vector k and a vector k0 (so k0 is axis Z) and is calculated from k0  (axis Z) to k - in this case when SinThata ==1 means that theta == 0 or theta == M_PI (so k lie along k0).
			 * Now we have relation: cos(ksi) = sin(theta)*cos(phi) [please look to the mentioned article above]
			 * An angle ksi is angle between k and e0 - in case when theta == 0 or theta == M_PI an angle ksi must be M_PI/2 (because axis Z is perpendicular to e0).
			 * In this situation we have equality 0 == 0*cos(phi) so phi can be equaled to any value - that why we use here uniform distribution.
			 * */
			phi = M_PI*(2.0*G4UniformRand()-1.0);
		}else{
			phi = std::acos(k_x/sinTheta);
		}

		if(k_y < 0)
			phi *= -1.0;
	}

	//Convert from radian to degree
	theta *= 180.0/M_PI;
	phi *= 180.0/M_PI;

	if(mAllPhiCalcNoNegative && phi < 0)
		phi += + 180.0;
}

G4double GateJPETGammaPolarizationActor::GetPolaizationAngle(const G4ThreeVector& k, const G4ThreeVector& e0, const G4ThreeVector& e)
{
	G4double polarization_angle = 0;
	if(mSaveTests && e0.mag() != 0){
		/** mScatteredGammaPolarizationAngle inform what was a new polarization after Compton's scatter.
		* */
		G4double dzeta = e0.angle(k);
		G4double huge_theta = e0.angle(e);
		G4double sin_dzeta,cos_huge_theta;
		sin_dzeta = sin(dzeta);
		cos_huge_theta = cos(huge_theta);
		G4double val = std::abs(sin_dzeta) <= pow(10,-6) ? 0.0 : cos_huge_theta/sin_dzeta;
		val = abs(val) == 1 ? 1.0 : val;
		polarization_angle = std::abs(sin_dzeta) <= pow(10,-6) ? 0.0 : std::acos(val);
		polarization_angle *= 180.0/M_PI;
	}

	return polarization_angle;
}

G4double GateJPETGammaPolarizationActor::GetAngleDifference(const G4ThreeVector& k, const G4ThreeVector& k0, const G4ThreeVector& e0)
{
	/** angular_diffrence_between_real_and_reconstructed_polarization inform about difference between real linear polarization angle (from simulation)
	* and polarization reconstructed as cross product of prime and scattered gamma momentum direction.
	* */
	G4double angular_diffrence_between_real_and_reconstructed_polarization = 0;
	if(mSaveTests && e0.mag() != 0)
	{
		angular_diffrence_between_real_and_reconstructed_polarization = e0.angle(k0.cross(k).unit());
		angular_diffrence_between_real_and_reconstructed_polarization *= 180.0/M_PI;
	}
	return angular_diffrence_between_real_and_reconstructed_polarization;
}

void GateJPETGammaPolarizationActor::AddETP(const G4Step *step)
{
	EventTrackPartner etp;
	etp.ComptonHappened = false;
	etp.PrimeGammaPolarization = step->GetPreStepPoint()->GetPolarization().unit();
	etp.PrimeGammaDirection = step->GetPreStepPoint()->GetMomentumDirection().unit();
	etp.PrimeTotalEnergy = step->GetPreStepPoint()->GetTotalEnergy();
	etp.PrimeEmissionKineticEnergy = step->GetTrack()->GetVertexKineticEnergy();
	etp.PrimePolarizationAngle = 0;

	if(etp.PrimeGammaPolarization.mag() > 0)
	{
		etp.PrimePolarizationAngle = std::acos(etp.PrimeGammaPolarization.dot(SetPerpendicularVector(etp.PrimeGammaDirection).unit()));
		etp.PrimePolarizationAngle *= 180.0/M_PI;
	}

	etp.PrimePolarizationAngle = 0;
	etp.PrimeEmissionPoint = step->GetTrack()->GetVertexPosition();
	etp.PrimeEmisionPhi = step->GetTrack()->GetVertexMomentumDirection().phi();
	etp.PrimeEmisionTheta = step->GetTrack()->GetVertexMomentumDirection().theta();
	mEventTracks[step->GetTrack()->GetTrackID()] = etp;
}

void  GateJPETGammaPolarizationActor::FillWithData(const G4int& eventID, const G4int& trackID, const EventTrackPartner* ptrETP)
{
	if(mUsePhiFilter)
		if((mPhiFilterLimes - ptrETP->ComptonPhi)*(mPhiFilterLimes - ptrETP->ComptonPhi) > mPhiFilterEpsilon*mPhiFilterEpsilon)
			return;

	if(mUseThetaFilter)
		if((mThetaFilterLimes - ptrETP->ComptonTheta)*(mThetaFilterLimes - ptrETP->ComptonTheta) > mThetaFilterEpsilon*mThetaFilterEpsilon)
			return;

	mPhi = ptrETP->ComptonPhi;
	mTheta  = ptrETP->ComptonTheta;
	mPrimeEnergy = ptrETP->PrimeEmissionKineticEnergy;

	if(mSaveHistograms)
	{
		pCrossSectionOfPhi->Fill(mPhi,1);
		pCrossSectionOfTheta->Fill(mTheta,1);
		pRealCrossSectionOfTheta->Fill(mTheta,1);
	}

	if(mSaveTests)
		mTests = ptrETP->Tests;

	pListeVar->Fill();

	if(mIsDiagnosticFileLoaded)
		SaveToFile(eventID, trackID, mPhi, mTheta, ptrETP->PrimePolarizationAngle, ptrETP->ComptonInteractionPoint, ptrETP->ComptonTotatEnergy, ptrETP->PrimeEmissionPoint, ptrETP->PrimeTotalEnergy, ptrETP->PrimeEmisionPhi,ptrETP->PrimeEmisionTheta);

}

void GateJPETGammaPolarizationActor::SetSaveOnlyWhenTheDesiredNumberOfParticlesHasScatteredFromEventEnable(G4bool enable)
{
	mSaveOnlyWhenTheDesiredNumberOfParticlesHasScatteredFromEvent = enable;
}

void GateJPETGammaPolarizationActor::SetDesiredNumberOfParticlesScatteredPerEvent(G4int particles_number)
{
	if( particles_number <= 0)
	{
		G4cerr<<"[WARNING] Desired number of particles scattered per event must be positive integer."<<G4endl;
		G4cerr<<"JPETGammaPolarizationActor will set default value equale "<<mDesiredNumberOfParticlesScatteredPerEvent<<G4endl;
		return;
	}
	mDesiredNumberOfParticlesScatteredPerEvent = particles_number;
}

void GateJPETGammaPolarizationActor::DisplaySummarizeBeforeRun()
{
	G4cout<<"*************************************************"<<G4endl;
	G4cout<<"JPETGammaPolarizationActor for volume \""<<GetVolumeName()<<"\" summarize: "<<G4endl;
	G4cout<<"Log file : "<<(mIsLogFileLoaded ? "ON" : "OFF")<<" "<<(mIsLogFileLoaded ? ". File name : "+mLogFileName : "")<<G4endl;
	G4cout<<"Diagnostic file : "<<(mIsDiagnosticFileLoaded ? "ON" : "OFF")<<" "<<(mIsDiagnosticFileLoaded ? ". File name : "+mDiagnosticFileName : "")<<G4endl;
	G4cout<<"PhiFilter : "<<(mUsePhiFilter ? "ON" : "OFF")<<" "<<(mUsePhiFilter ? ". Limes : "+std::to_string(mPhiFilterLimes) + " deg. Epsilon : "+std::to_string(mPhiFilterEpsilon)+" deg." : "")<<G4endl;
	G4cout<<"ThetaFilter : "<<(mUseThetaFilter ? "ON" : "OFF")<<" "<<(mUseThetaFilter ? ". Limes : "+std::to_string(mThetaFilterLimes) + " deg. Epsilon : "+std::to_string(mThetaFilterEpsilon)+" deg." : "")<<G4endl;
	G4cout<<"All phi positive : "<<(mAllPhiCalcNoNegative ? "ON" : "OFF")<<G4endl;
	G4cout<<"Angular Accuracy : "<<mAngularAccuracy<<" deg per bin"<<G4endl;
	G4cout<<"Saving histograms : "<<(mSaveHistograms ? "ON" : "OFF")<<G4endl;
	G4cout<<"Saving tests : "<<(mSaveTests ? "ON" : "OFF")<<G4endl;
	G4cout<<"Saving for desired number of particles per event : "<<(mSaveOnlyWhenTheDesiredNumberOfParticlesHasScatteredFromEvent ? "ON" : "OFF")<<G4endl;
	G4cout<<"Desired number of particles per event : "<<mDesiredNumberOfParticlesScatteredPerEvent<<G4endl;
	G4cout<<"*************************************************"<<G4endl;
}
