/**
 *  @copyright Copyright 2018 The J-PET Gate Authors. All rights reserved.
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
 *  @file GateGlobalActor.hh
 */
#ifndef GATEGLOBALACTOR_HH
#define GATEGLOBALACTOR_HH
#include "TTree.h"
#include "TFile.h"
#include "G4Step.hh"
#include "G4String.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include <map>
#include <set>
#include <memory>
#include "TVector3.h"

/**
 * @About class: This class represent global data acquisition actor. This actor is substitute for sensitive detectors - crystalSD and phantomSD.
 * Compared to cristalSD and phantomSD this actor provide method to filtering and saving data - you can what save and when.
 * Class GateLocalActor is node of GateGlobalActor.
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateGlobalActor
{
	//This type represent checking functions pointers
	typedef bool(GateGlobalActor::*CheckFunction)(const G4Step&) const;
	//This type represent updating methods pointers
	typedef void(GateGlobalActor::*UpdateMethod)(const G4Step&);

public:
	/** Destructor
	 * */
	virtual ~GateGlobalActor();

	/** This function return GateGlobalActor pointer.
	 * By using this function you receive access to this actor.
	 * */
	static GateGlobalActor* Instance();

	/** Function call writing data to file.
	 * */
	void Write();

	/** Function call reseting tree method.
	 * */
	void Reset();

	/** This method is for communicate LocalActors with GlobalActor.
	 * @param: volume_name - name of volume to which is attached actor (you can receive access to this by calling GetVolumeName())
	 * @param: step - step pointer
	 * */
	void NoticeStep(const G4String& volume_name, const G4Step* step);

	/** Set destination data file name (ROOT file)
	 * @param: file_name - root file name (with or without ".root" ending
	 * */
	void SetFileName(const G4String& file_name);

private:
	/** Constructor
	 * */
	GateGlobalActor();
	//Singleton object
	static std::unique_ptr<GateGlobalActor> upInstance;
	//Tree with data. This tree name is "GateGlobalActorTree".
	TTree* pTree;
	//ROOT file
	TFile* pFile;
	//List with all checking function which was chosen by user (by calling SetFilter method)
	std::map<G4String, CheckFunction> mCheckFunctionsPointersList;
	//List with all update method which was chosen by user (by calling SetEnable method)
	std::map<G4String, UpdateMethod> mUpdateMethodsPointersList;
	//ROOT file name - by default is "GateGlobalActorData.root".
	G4String mFileName;

private:

	/** Method create ROOT file basing on file name.
	 * @param: file_name - ROOT file name
	 * */
	void InitFile(const G4String& file_name);

	/** Method create tree.
	 * */
	void InitTree();

	/** Method call checking functions until one of them return false or checking functions list finish.
	 * @param: step - step pointer
	 * @return: TRUE - if one of checking function return FALSE, otherwise this function returns FALSE.
	 * */
	G4bool SkipThisStep(const G4Step* step);

	/** This function basing on step and update functions list updates variables.
	 * @param: step - step pointer
	 * */
	void UpdateFromThisStep(const G4Step* step);

	/** This function just fill tree.
	 * */
	void FillTree();

	/** Use this method to add new update method to list.
	 * @param: update_method_name - your update method name
	 * @param: update_method - your method - add it by set &GateGlobalActor::YourUpdateMethod
	 * */
	void TryAddUpdateMethod(const G4String& update_method_name, const UpdateMethod& update_method);

	/** Use this function to add new branch to tree.
	 * @param: branch_name - your branch name (please add branch understandable name)
	 * @param: variable - just add put here your variable
	 * */
	template <class T>
	void TryAddBranch(const G4String& branch_name, T& variable);

	/** Use this to add new checking function to list.
	 * @param: check_function_name - your cheching function name
	 * @param: check_function - your function - add it by set &GateGlobalActor::YourCheckFunction
	 * */
	void TryAddCheckFunction(const G4String& check_function_name, const CheckFunction& check_function);

	/** Function tries to add new element to set - if element exist in set then nothing will happen.
	 * @param: set - set
	 * @param: value_to_add - value which probably will be added to set
	 * */
	template <class T>
	void TryAddToSet(std::set<T>& set, const T& value_to_add);

	/** Convert radians to degree
	 * @param: angle_radians - angle in radians
	 * @return: angle in degree
	 * */
	G4double deg(const G4double& angle_radians) const;

	/** Convert energy in MeV (default energy unit in Geant4 and GATE) to keV.
	 * @param: energy_MeV - energy in MeV (just energy from GATE and Geant4)
	 * @return: energy in keV
	 * */
	G4double keV(const G4double& energy_MeV) const;

	/** Convert G4ThreeVector to TVector3 - use this function when is need to save data as TVector3
	 * @param: from - input G4ThreeVector
	 * @param: to - output TVector3
	 * */
	void ConvertToTVector3(const G4ThreeVector& from, TVector3& to);

//@Section: Control of saving data
public:
	//@SubSection: SetEnable methods

	void SetEnableVolumeName();

	void SetEnableScintilatorPosition();

	void SetEnableEventID();

	void SetEnableTrackID();

	void SetEnableEnergyBeforeProcess();

	void SetEnableEnergyAfterProcess();

	void SetEnableEnergyLossDuringProcess();

	void SetEnableMomentumDirectionBeforeProcess();

	void SetEnableMomentumDirectionAfterProcess();

	void SetEnableProcessPosition();

	void SetEnableEmissionPointFromSource();

	void SetEnableEmissionMomentumDirectionFromSource();

	void SetEnableEmissionEnergyFromSource();

	void SetEnableParticleName();

	void SetEnableParticlePGDCoding();

	void SetEnableProcessAngle();

	void SetEnablePolarizationBeforeProcess();

	void SetEnablePolarizationAfterProcess();

	void SetEnableProcessName();

	void SetEnableParentID();

	void SetEnableInteractionTime();

	void SetEnableLocalTime();

	void SetEnableGlobalTime();

	void SetEnableProperTime();

private:
	//@SubSection: Update methods

	void UpdateScintilatorPosition(const G4Step& step);

	void UpdateEventID(const G4Step& step);

	void UpdateTrackID(const G4Step& step);

	void UpdateEnergyBeforeProcess(const G4Step& step);

	void UpdateEnergyAfterProcess(const G4Step& step);

	void UpdateEnergyLossDuringProcess(const G4Step& step);

	void UpdateMomentumDirectionBeforeProcess(const G4Step& step);

	void UpdateMomentumDirectionAfterProcess(const G4Step& step);

	void UpdateProcessPosition(const G4Step& step);

	void UpdateEmissionPointFromSource(const G4Step& step);

	void UpdateEmissionMomentumDirectionFromSource(const G4Step& step);

	void UpdateEmissionEnergyFromSource(const G4Step& step);

	void UpdateParticleName(const G4Step& step);

	void UpdateParticlePGDCoding(const G4Step& step);

	void UpdateProcessAngle(const G4Step& step);

	void UpdatePolarizationBeforeProcess(const G4Step& step);

	void UpdatePolarizationAfterProcess(const G4Step& step);

	void UpdateProcessName(const G4Step& step);

	void UpdateParentID(const G4Step& step);

	void UpdateInteractionTime(const G4Step& step);

	void UpdateLocalTime(const G4Step& step);

	void UpdateGlobalTime(const G4Step& step);

	void UpdateProperTime(const G4Step& step);

private:
	//@SubSection: Variables connected with tree branches

	std::string mVolumeName;

	TVector3 mScintilatorPosition;

	G4int mEventID;

	G4int mTrackID;

	G4double mEnergyBeforeProcess;

	G4double mEnergyAfterProcess;

	G4double mEnergyLossDuringProcess;

	TVector3 mMomentumDirectionBeforeProcess;

	TVector3 mMomentumDirectionAfterProcess;

	TVector3 mProcessPosition;

	TVector3 mEmissionPointFromSource;

	TVector3 mEmissionMomentumDirectionFromSource;

	G4double mEmissionEnergyFromSource;

	std::string mParticleName;

	G4int mParticlePGDCoding;

	G4double mProcessAngle;

	TVector3 mPolarizationBeforeProcess;

	TVector3 mPolarizationAfterProcess;

	std::string mProcessName;

	G4int mParentID;

	G4double mInteractionTime;

	G4double mLocalTime;

	G4double mGlobalTime;

	G4double mProperTime;

public:
	//@SubSection: SetFilter

	/* Set new process name filter.
	 * @param: process_name - name of process which you want filter out
	 * @IsReusable: Yes
	 * */
	void SetFilterProcessName(const G4String& process_name);

	/* Set new particle name filter.
	 * @param: particle_name - name of particle to filter out
	 * @IsReusable: Yes
	 * */
	void SetFilterParticleName(const G4String& particle_name);

	/* Set new particle PDG code filter.
	 * @param: pdg_code - PDG code of particle which you want filter out
	 * @IsReusable: Yes
	 * */
	void SetFilerParticlePDGCode(const G4int& pdg_code);

	/* Set new process angle (angle between particle momentum direction before and after process) name filter.
	 * @param: angle - angle between particle momentum direction before and after process which you want filter out
	 * @IsReusable: No
	 * */

	void SetFilerProcessAngle(const G4double& angle);
	/* Set new particle emission point filter.
	 * @param: emission_point- particle emission point form source which you want filter out
	 * @IsReusable: No
	 * */
	void SetFilterEmissionPoint(const G4ThreeVector& emission_point);

	/* Set filter which ignore sepecific process (for example does not save steps with Compton process)
	 * @param: process_name - name of process which you want ingore during simulation
	 * @IsReusable: Yes
	 * */
	void SetFilterIgnoreProcessName(const G4String& process_name);

private:
	//@SubSection: Check functions

	/** Check if process name from step is on filter list
	 * @return: TRUE - when is it, FALSE - otherwise
	 * */
	G4bool CheckProcessName(const G4Step& step) const;

	/** Check if particle name from step is on filter list
	 * @return: TRUE - when is it, FALSE - otherwise
	 * */
	G4bool CheckParticleName(const G4Step& step) const;

	/** Check if particle PDG code from step is on filter list
	 * @return: TRUE - when is it, FALSE - otherwise
	 * */
	G4bool CheckPDGCode(const G4Step& step) const;

	/** Check if angle between particle momentum direction before and after is equal filter value (in small error range)
	 * @return: TRUE - when is it, FALSE - otherwise
	 * */
	G4bool CheckProcessAngle(const G4Step& step) const;

	/** Check if distance between particle emission point is equal filter value (in small error range)
	 * @return: TRUE - when is it, FALSE - otherwise
	 * */
	G4bool CheckEmissionPoint(const G4Step& step) const;

	/** Check if process name from step is on ignoring filter list
	 * @return: TRUE - when is it, FALSE - otherwise
	 * */
	G4bool CheckIgnoreProcessName(const G4Step& step) const;

private:
	//@SubSection: Variables which represent filters (filter can be represented by list of variables e.g. list of partciles names)
	//Using of set<> is recommended because of log complexity of set<> (vector has to use std::find which has linear complexity)

	std::set<G4String> mFilterProcessesNames;
	std::set<G4String> mFilterParticleName;
	std::set<int> mFilterPDGCodes;
	G4double mFilterProcessAngle;
	G4ThreeVector mFilterEmissionPoint;
	std::set<G4String> mFilterIgnoreProcessesNames;
};

#endif
