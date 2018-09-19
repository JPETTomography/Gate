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
 *  @file GateJPETGammaPolarizationActor.hh
 */

#include "GateConfiguration.h"

#ifndef GATEJPETGAMMAPOLARIZATIONACTOR_HH
#define GATEJPETGAMMAPOLARIZATIONACTOR_HH

#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"
#include<fstream>
#include "TH2.h"
#include "TH1.h"
#include<map>

#include "GateVActor.hh"
#include "GateJPETGammaPolarizationActorMessenger.hh"

struct iaea_header_type;
struct iaea_record_type;


/**
 * Actor created just for gamma quanta's polarization analysis in Compton process (for JPET use)
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateJPETGammaPolarizationActor : public GateVActor
{
public: //Functions, structures and enums
	/** Simple structure which represent branch with test for polarization.
	 * */
	struct PolarizationControlTests{
		//Angle on linear polarization on plane orthogonal to gamma direction
		G4double ScatteredGammaPolarizationAngle;
		//Angular difference between polarization received from simulation and as a cross product of prime and scattered gamma (JPET idea)
		G4double AngularDiffreneceBetweenRealAndReconstructedPrimeGammaPolarization;
	};

	/** This structure contain all information about track before and after scattering.
	 * The purpose of this is to provide simple way to save data only when is needed to files
	 * */
	struct EventTrackPartner{
		G4bool ComptonHappened;

		//Information about particle before Compton's scattering
		G4ThreeVector PrimeGammaPolarization;
		G4ThreeVector PrimeGammaDirection;
		G4double PrimeTotalEnergy;
		G4double PrimePolarizationAngle;

		//Information about particle when was emitted from source
		G4ThreeVector PrimeEmissionPoint;
		G4double PrimeEmisionPhi;
		G4double PrimeEmisionTheta;
		G4double PrimeEmissionKineticEnergy;

		//Information about particle after Compton's scattering
		G4double ComptonPhi;
		G4double ComptonTheta;
		G4ThreeVector ComptonInteractionPoint;
		G4double ComptonTotatEnergy;
		PolarizationControlTests Tests;
	};

	enum PhiThetaValueMode{
		Flag,
		Limes,
		Epsilon
	};

	virtual ~GateJPETGammaPolarizationActor();
	FCT_FOR_AUTO_CREATOR_ACTOR(GateJPETGammaPolarizationActor)
	// Constructs the sensor
	virtual void Construct() override;
	// Callbacks
	virtual void UserSteppingAction(const GateVVolume *, const G4Step*) override;

	/** Saves the data collected to the file
	 */
	virtual void SaveData() override;
	/** Overwrite function from GateVActor.
	 * This function used by GateVActor::BeginOfRunAction. If you want to reset data in file for each new run, call EnableResetDataAtEachRun(true).
	 * By default this function is not used;
	 */
	virtual void ResetData() override;

	/**Set log file
	 * @param: log_file_name - log file name
	 * */
	void SetLogFile(G4String log_file_name);

	/** Set saving numerical data to ASCI file
	 * @param: diag_file_name - ASCI file name
	 * */
	void SetDiagnosticFile(G4String diag_file_name);

	/** Set filtration range for phi
	 * @param: value - value to set - depends on what is mode you have to input other value (for more look to function body)
	 * @param: mode - mode inform how to interpret value
	*/
	void SetPhiFlagParameter(G4double value, GateJPETGammaPolarizationActor::PhiThetaValueMode mode);

	/** Set filtration range for theta
	 * @param: value - value to set - depends on what is mode you have to input other value (for more look to function body)
	 * @param: mode - mode inform how to interpret value
	*/
	void SetThetaFlagParameter(G4double value, GateJPETGammaPolarizationActor::PhiThetaValueMode mode);

	/** Set filtration range for phi
	 * @param: setAsPlus - if is necessary to save phi angle as only positive value this flag must be set as true
	*/
	void SetAllPhiAsNoNegative(G4bool setAsPlus);

	/** Set generation of histogram as (dis)enable.
	 * By default generation is enable;
	 * @param: enable - set false to disable histograms generations
	*/
	void SetHistogramsEnable(G4bool enable);

	/** Set generation of tests branch (for polarization) as (dis)enable.
	 * By default generation is enable;
	 * @param: enable - set false to disable tests branch generations
	*/
	void SetTestsBranchEnable(G4bool enable);

	/** Set angle precision per bin in histograms - this mean that each bin has width equal angle_precison
	 * By default precision is 2 degree.
	 * @param: angle_precision - bin width (in degrees)
	 * @exceptions: If value is to huge, what mean angle_precision >= 180.0 deg then this value is ignored.
	*/
	void SetAnglePrecisionPerBinInHistograms(G4double angle_precision);

	/** Set (dis)enable saving data ony for desired number of particles per event which scattered.
	 * The purpose of this to reduce number of data generated during simulation.
	 * @param: enable - set true to use this, false - otherwise
	 * */
	void SetSaveOnlyWhenTheDesiredNumberOfParticlesHasScatteredFromEventEnable(G4bool enable);

	/** Set desired number of particles scattered per event.
	 * @param: particles_number - if number of scattered per event is equal particles_number then data will be saved
	 * */
	void SetDesiredNumberOfParticlesScatteredPerEvent(G4int particles_number);

private: //Functions
	/** Function generate perpendicular vector to vector 'a'
	 * @param: a - vector for which we create perpendicular one
	 * @return: perpendicular vector to 'a'
	*/
	G4ThreeVector SetPerpendicularVector(const G4ThreeVector& a);

	/* Special function just for this class.
	* Extract values setted by Set functions.
	* */
	void StandardExtractFunction(const G4Step *step);

	/** Function generate dsigma/dOmega (theta) - that is cross-section of theta (angle between prime and scattered gamma).
	 * For this this function generate histogram which is histogram dsigma/dOgema divided by sin(theta) histogram.
	 * */
	void GenerateRealCrossSectionOfTheta();

	/** Function normalize major of histograms - for more look to function body
	 * */
	void NormalizeHistograms();

	/** Function save numerical data to ASCII file
	 * */
	void SaveToFile(const G4int& eventID, const G4int& trackID, const G4double& phi, const G4double& theta, const G4double& polarization, const G4ThreeVector& interactionPoint, const G4double& totalEnergy, const G4ThreeVector& emisionPoint, const G4double& primeEnergy, const G4double& emisionPhi, const G4double& emisionTheta);

	/** Function calculate phi and theta for scattered gamma.
	 * @param: k0 - prime gamma momentum direction
	 * @param: k - scattered gamma momentum direction
	 * @param: e0 - prime gamma polarization
	 * @param: theta - angle between k0 and k
	 * @param: phi - angle between e0 and k projection on plane created by e0 and e0xk0
	 * */
	void GetThetaAndPhi(const G4ThreeVector& k0, const G4ThreeVector& k, const G4ThreeVector& e0, G4double& theta, G4double& phi);

	/** Function calculate angle of linear polarization
	 * @param: k0 - prime gamma momentum direction
	 * @param: e0 - prime gamma polarization
	 * @param: e - scattered gamma polarization
	 * @return: linear polarization angle (in degree)
	 * */
	G4double GetPolaizationAngle(const G4ThreeVector& k, const G4ThreeVector& e0, const G4ThreeVector& e);

	/** Function calculate difference between real and reconstructed prime polarization vectors
	 * @param: k0 - prime gamma momentum direction
	 * @param: k - scattered gamma momentum direction
	 * @param: e0 - prime gamma polarization
	 * */
	G4double GetAngleDifference(const G4ThreeVector& k, const G4ThreeVector& k0, const G4ThreeVector& e0);

	/** Function save data to histograms, tree and ASCII file
	 * @param: eventID - current event ID
	 * @param: trackID - track ID connected with data from ETP
	 * @param: ptrETP - pointer to ETP structure
	 * */
	void FillWithData(const G4int& eventID, const G4int& trackID, const EventTrackPartner* ptrETP);

	/** Function create new element of mEventTracks basing on step data.
	 * @param: step - current step
	 * */
	void AddETP(const G4Step *step);

	/** Before start simulation Actor display information about parameters
	 * */
	void DisplaySummarizeBeforeRun();

protected: //Functions
	/** Constructor
	 * */
	GateJPETGammaPolarizationActor(G4String name, G4int depth=0);

private: //Variables - please group them by type and aim of use

	//Saving to ASCII files

	G4String mLogFileName;
	G4String mDiagnosticFileName;

	G4bool mIsLogFileLoaded;
	G4bool mIsDiagnosticFileLoaded;

	std::fstream mLOG;
	std::fstream mDGN;

	//Saving to root file

	TFile * pFile;
	TTree * pListeVar;

	//Messenger

	GateJPETGammaPolarizationActorMessenger * pMessenger;

	//Histograms

	//Histogram of dsigma/dphi (phi) == dsigma/dOmega (phi)
	TH1F* pCrossSectionOfPhi;
	//Histogram of dsigma/dtheta (theta)
	TH1F* pCrossSectionOfTheta;
	//Histogram of dsigma/dOmega (theta) - this histogram is calculated at the end of actor work
	TH1F* pRealCrossSectionOfTheta;
	//Histogram show how many particles scattered per event
	TH1F* pAllParticlesReacted;

	//Variables connected with saving to root file

	G4double mPhi;
	G4double mTheta;
	G4double mPrimeEnergy;

	PolarizationControlTests mTests;

	//Flags

	G4bool mUsePhiFilter;
	G4bool mUseThetaFilter;
	G4bool mSaveHistograms;
	G4bool mSaveTests;
	G4bool mAllPhiCalcNoNegative;
	G4bool mSaveOnlyWhenTheDesiredNumberOfParticlesHasScatteredFromEvent;

	G4double mPhiFilterLimes;
	G4double mPhiFilterEpsilon;
	G4double mThetaFilterLimes;
	G4double mThetaFilterEpsilon;
	G4double mAngularAccuracy;

	G4int mDesiredNumberOfParticlesScatteredPerEvent;

	//Normal variables

	G4int mEventID;
	G4int mScatteredParticlesPerEvent;

	std::map<G4int, EventTrackPartner> mEventTracks;

};

MAKE_AUTO_CREATOR_ACTOR(JPETGammaPolarizationActor,GateJPETGammaPolarizationActor)

#endif
