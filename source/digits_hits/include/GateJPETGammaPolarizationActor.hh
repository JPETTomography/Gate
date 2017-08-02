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

	void SetPhiFlagParameter(double value, GateJPETGammaPolarizationActor::PhiThetaValueMode mode);

	void SetThetaFlagParameter(double value, GateJPETGammaPolarizationActor::PhiThetaValueMode mode);

	void SetAllPhiAsNoNegative(bool setAsPlus);

protected:
	GateJPETGammaPolarizationActor(G4String name, G4int depth=0);

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
	double mAnglePrimeAndScatteredGammaPolarizationVectorsAfterUzRotate;
	double mAnglePerpendicularAndScatteredGammaPolarizationVectorsAfterUzRotate;

	double mPhi, mTheta;

	//Stokes parameters (set of values that describe the polarization state of electromagnetic radiation)- mSx = S1, mSy = S2, mSz = S3
	double mSx, mSy, mSz;

	bool mUsePhiFilter, mUseThetaFilter;
	double mPhiFilterLimes, mPhiFilterEpsilon, mThetaFilterLimes, mThetaFilterEpsilon;

	bool mAllPhiCalcNoNegative;

	//Variables need to save data
	TFile * pFile;
	TTree * pListeVar;
	GateJPETGammaPolarizationActorMessenger * pMessenger;

	//Tracing tools variables
	bool mIsFirstStep;

	/* Special function just for this class.
	 * Extract values setted by Set functions.
	 * */
	void StandardExtractFunction(const G4Step *step);

	//Tool's flags
	int mEventID;
	bool mComptonHappened;
	G4ThreeVector mPrimePolarization;
	bool mSkipThis;
};

MAKE_AUTO_CREATOR_ACTOR(JPETGammaPolarizationActor,GateJPETGammaPolarizationActor)

#endif
