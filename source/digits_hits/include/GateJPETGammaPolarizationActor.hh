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
public:
	/** Simple structure which represent branch with test for polarization.
	 * */
	struct PolarizationControlTests{
		//Angle on linear polarization on plane orthogonal to gamma direction
		double ScatteredGammaPolarizationAngle;
		//Angular difference between polarization received from simulation and as a cross product of prime and scattered gamma (JPET idea)
		double AngularDiffreneceBetweenRealAndReconstructedPrimeGammaPolarization;
	};

	enum PhiThetaValueMode{
		Flag,
		Limes,
		Epsilon
	};

	virtual ~GateJPETGammaPolarizationActor();
	FCT_FOR_AUTO_CREATOR_ACTOR(GateJPETGammaPolarizationActor)
	// Constructs the sensor
	virtual void Construct();
	// Callbacks
	virtual void UserSteppingAction(const GateVVolume *, const G4Step*);
	virtual void PreUserTrackingAction(const GateVVolume *, const G4Track*);
	virtual void BeginOfEventAction(const G4Event * e);

	/** Saves the data collected to the file
	 */
	virtual void SaveData();
	/** Overwrite function from GateVActor.
	 * This function used by GateVActor::BeginOfRunAction. If you want to reset data in file for each new run, call EnableResetDataAtEachRun(true).
	 * By default this function is not used;
	 */
	virtual void ResetData();

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
	void SetPhiFlagParameter(double value, GateJPETGammaPolarizationActor::PhiThetaValueMode mode);

	/** Set filtration range for theta
	 * @param: value - value to set - depends on what is mode you have to input other value (for more look to function body)
	 * @param: mode - mode inform how to interpret value
	*/
	void SetThetaFlagParameter(double value, GateJPETGammaPolarizationActor::PhiThetaValueMode mode);

	/** Set filtration range for phi
	 * @param: setAsPlus - if is necessary to save phi angle as only positive value this flag must be set as true
	*/
	void SetAllPhiAsNoNegative(bool setAsPlus);

	/** Set generation of histrogram as (dis)enable.
	 * By default generation is enable;
	 * @param: enable - set false to disable histograms generations
	*/
	void SetHistogramsEnable(bool enable);

	/** Set generation of tests branch (for polarization) as (dis)enable.
	 * By default generation is enable;
	 * @param: enable - set false to disable tests branch generations
	*/
	void SetTestsBranchEnable(bool enable);

	/** Set angle precision per bin in histograms - this mean that each bin has width equale angle_precison
	 * By default precision is 2 degree.
	 * @param: angle_precision - bin width (in degrees)
	 * @exceptions: If value is to huge, what mean angle_precision >= 180.0 deg then this value is ignored.
	*/
	void SetAnglePrecisionPerBinInHistograms(double angle_precision);

protected:
	/** Constructor
	*/
	GateJPETGammaPolarizationActor(G4String name, G4int depth=0);

	/** Function generate perpendicular vector to vector 'a'
	 * @param: a - vector for which we create perpendicular one
	 * @return: perpendicular vector to 'a'
	*/
	G4ThreeVector SetPerpendicularVector(G4ThreeVector& a);
	
	//Saving
	G4String mLogFileName;
	G4String mDiagnosticFileName;
	bool mIsLogFileLoaded;
	bool mIsDiagnosticFileLoaded;

	std::fstream mLOG;
	std::fstream mDGN;

	//Saving variables
	double mAnglePrimeAndScatteredGammaPolarizationVectors; //deg
	double mAnglePrimeAndScatteredGammaMomentumVectors; //deg
	double mAnglePrimeAndScatteredGammaPolarizationVectorsAfterUzRotate; //deg
	double mAnglePerpendicularAndScatteredGammaPolarizationVectorsAfterUzRotate; //deg

	double mPhi, mTheta;

	bool mUsePhiFilter, mUseThetaFilter;
	double mPhiFilterLimes, mPhiFilterEpsilon, mThetaFilterLimes, mThetaFilterEpsilon;

	bool mAllPhiCalcNoNegative;

	PolarizationControlTests mTests;

	//Variables need to save data
	TFile * pFile;
	TTree * pListeVar;
	TH2F *hpxpy;
	GateJPETGammaPolarizationActorMessenger * pMessenger;

	//Histograms saved to file
	//Histogram of dsigma/dphi (phi) == dsigma/dOmega (phi)
	TH1F* pCrossSectionOfPhi;
	//Histogram of dsigma/dtheta (theta)
	TH1F* pCrossSectionOfTheta;
	//Histogram of dsigma/dOmega (theta) - this histogram is calculated at the end of actor work
	TH1F* pRealCrossSectionOfTheta;

	//Histogram params
	double mAngularAccuracy;

	//Save controls flags

	bool mSaveHistograms;
	bool mSaveTests;

	//Tracing tools variables
	bool mIsFirstStep;

	/* Special function just for this class.
	 * Extract values setted by Set functions.
	 * */
	void StandardExtractFunction(const G4Step *step);
	void GenerateRealCrossSectionOfTheta();
	void NormalizeHistograms();

	//Tool's flags
	int mEventID;
	bool mComptonHappened;
	G4ThreeVector mPrimeGammaPolarization;
	G4ThreeVector mPrimeGammaDirection;
};

MAKE_AUTO_CREATOR_ACTOR(JPETGammaPolarizationActor,GateJPETGammaPolarizationActor)

#endif
