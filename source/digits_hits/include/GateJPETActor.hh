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
 *  @file GateJPETActor.hh
 */

#include "GateConfiguration.h"

#ifndef GATEJPETACTOR_HH
#define GATEJPETACTOR_HH

#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"

#include "GateVActor.hh"
#include "GateJPETActorMessenger.hh"

#include<fstream>

struct iaea_header_type;
struct iaea_record_type;

/**
 * Actor created for JPET use
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateJPETActor : public GateVActor
{
public:
	virtual ~GateJPETActor();
	// This macro initialize the CreatePrototype and CreateInstance
	FCT_FOR_AUTO_CREATOR_ACTOR(GateJPETActor)
	// Constructs the sensor
	virtual void Construct() override;
	// Callbacks
	virtual void UserSteppingAction(const GateVVolume *, const G4Step*) override;
	virtual void PreUserTrackingAction(const GateVVolume *, const G4Track*) override;
	virtual void BeginOfEventAction(const G4Event * e) override;

	/** Saves the data collected to the file
	 */
	virtual void SaveData() override;
	/** Overwrite function from GateVActor.
	 * This function used by GateVActor::BeginOfRunAction. If you want to reset data in file for each new run, call EnableResetDataAtEachRun(true).
	 * By default this function is not used;
	 */
	virtual void ResetData() override;

	///Setting functions - use it if want activate or deactivate saving some data

	/**
	 * Record data about x - position
	 * @param: enableXPosition - set true if u want have data of particle x-position
	 */
	void SetXPositionEnabled(bool enableXPosition);

	/**
	 * Record data about y - position
	 * @param: enableYPosition - set true if u want have data of particle y-position
	 */
	void SetYPositionEnabled(bool enableYPosition);

	/**
	 * Record data about z - position
	 * @param: enableZPosition - set true if u want have data of particle z-position
	 */
	void SetZPositionEnabled(bool enableZPosition);

	/**
	 * Record data about particle Name
	 * @param: enableParticleName
	 */
	void SetParticleNameEnabled(bool enableParticleName);

	/**
	 * Choose for which particle record data
	 * @param: particleName - set particle name from enabled names in G4 e.g. e-,e+,gamma, etc.
	 * @ATTENTION!: If particleName is incorrect any data will be generated
	 * @TIP: U can use more then once this function to add more particles to recored
	 * */
	void SetParticleEnabled(G4String particleName);

	/**
	 * Record data about particle trackID
	 * @param: enableTrackID - set true if u want have data about this
	 */
	void SetTrackIDEnabled(bool enableTrackID);

	/**
	 * Record data about particle eventID
	 * @param: enableEventID - set true if u want have data abut this
	 */
	void SetEventIDEnabled(bool enableEventID);

	/**
	 * Record data about particle ParentID
	 * @param: enableParentID - set true if u want have data abut this
	 */
	void SetParentIDEnabled(bool enableParentID);

	/**
	 * Record data about particle RunID
	 * @pram: enableRunID - set true if u want have data abut this
	 */
	void SetRunIDEnabled(bool enableRunID);

	/**
	 * Record data about particle kinetic energy Ekin
	 * @param: enableEkin - set true if u want have data abut this
	 */
	void SetEkinEnabled(bool enableEkin);

	/**
	 * Record data about partciel interaction time
	 * @param: enableInteractionTime - set true if u want have data abut this
	 */
	void SetInteractionTimeEnabled(bool enableInteractionTime);

	/**
	 * Record data about Process Name witch conected is with particle
	 * @param: enableProcessName - set true if u want have data abut this
	 */
	void SetProcessNameEnabled(bool enableProcessName);

	/**
	 * Record data about emission point
	 * @param: enableEmissionPoint - set true if u want have data abut this
	 */
	void SetEmissionPointEnabled(bool enableEmissionPoint);

	/**
	 * Record data about primary energy
	 * @param: enablePrimaryEnergy - set true if u want have data abut this
	 */
	void SetPrimaryEnergy(bool enablePrimaryEnergy);

	/**
	 * Use ASCI file to save partcile data about name and positions
	 * @param: fileName - ASCI file name 
	 */
	void SetASCIFileName(std::string fileName);

protected:
	GateJPETActor(G4String name, G4int depth=0);

	//Setting values showing which data record and which not - ATTENTION!: In defaul mode all bool setting are FALSE and vector is EMPTY
	/*Record data about x-position*/
	bool mEnableXPosition;
	/*Record data about y-position*/
	bool mEnableYPosition;
	/*Record data about z-position*/
	bool mEnableZPosition;
	/*Record data about particle name*/
	bool mEnableParticleName;
	/*Record data for particles from THIS list. ATTENTION: If this vector is empty class will be record data for every particles which fullfil conditions*/
	std::vector<G4String> mParticleNames;
	/*Record track ID*/
	bool mEnableTrackID;
	/*Record Event ID*/
	bool mEnableEventID;
	/*Record Parent ID*/
	bool mEnableParentID;
	/*Record Run ID*/
	bool mEnableRunID;
	/*Record Kinetic energy of particle*/
	bool mEnableKineticEnergy;
	/*Record interaction time*/
	bool mEnableInteractionTime;
	/*Record process name*/
	bool mEnableProcessName;
	/*Record emission point*/
	bool mEnableEmissionPoint;
	/*Record primary energy*/
	bool mEnablePrimaryEnergy;

	//Values
	double mPositionX;
	double mPositionY;
	double mPositionZ;
	Char_t mParticleName[256];
	int mTrackID;
	int mEventID;
	int mRunID;
	int mParentID;
	double mKinecticEnergy;
	double mInteractionTime;
	Char_t mProcessName[256];
	double mEmissionPositionX;
	double mEmissionPositionY;
	double mEmissionPositionZ;
	double mPrimaryEnergy;
	//Variables need to save data
	TString mFileType;
	TFile* pFile;
	TTree*  pListeVar;
	GateJPETActorMessenger* pMessenger;
	//Saving data to ASCII files
	std::fstream mASCIFile;
	std::string mASCIFileName;

	//Tracing tools variables
	bool mIsFirstStep;

	//Special function just for this class
	/*
	 * Extract values setted by Set functions
	 * */
	void StandardExtractFunction(const G4Step *step);

};

MAKE_AUTO_CREATOR_ACTOR(JPETActor,GateJPETActor)

#endif /* end #define GATEJPETACTOR_HH */
