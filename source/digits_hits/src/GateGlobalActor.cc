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
 *  @file GateGlobalActor.cc
 */
#include "GateGlobalActor.hh"
#include <cassert>
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4String.hh"
#include <cstring>
#include "GateRunManager.hh"
#include "G4Track.hh"

std::unique_ptr<GateGlobalActor> GateGlobalActor::upInstance;

GateGlobalActor::GateGlobalActor()
{
	pTree = 0;
	pFile = 0;
	mFileName = "GateGlobalActorData.root";
}

GateGlobalActor::~GateGlobalActor()
{

}

GateGlobalActor* GateGlobalActor::Instance()
{
	if(upInstance.get() == nullptr)
		upInstance.reset(new GateGlobalActor());
	return upInstance.get();
}

void GateGlobalActor::Write()
{
	assert(pFile != nullptr);
	if(pTree == nullptr)
		return;
	pFile = pTree->GetCurrentFile();
	pFile->Write();
}

void GateGlobalActor::Reset()
{
	if(pTree == nullptr)
		return;
	pTree->Reset();
}

void GateGlobalActor::NoticeStep(const G4String& volume_name, const G4Step* step)
{
	if(SkipThisStep(step))
		return;
	mVolumeName = volume_name;
	UpdateFromThisStep(step);
	FillTree();
}

void GateGlobalActor::SetFileName(const G4String& file_name)
{
	G4cout<<"SetFileName"<<G4endl;

	assert(file_name.size() > 0);
	if(file_name.rfind(".root") == std::string::npos)
		mFileName = file_name + ".root";
	else
		mFileName = file_name;
}

void GateGlobalActor::InitFile(const G4String& file_name)
{
	assert(file_name.size() > 0);

	assert(pFile == nullptr);

	if(file_name.rfind(".root") == std::string::npos)
		pFile = new TFile( (file_name + std::string(".root")).c_str(), "RECREATE");
	else
		pFile = new TFile( file_name.c_str(), "RECREATE");
}

void GateGlobalActor::GateGlobalActor::InitTree()
{
	assert(pTree == nullptr);
	pTree = new TTree("GateGlobalActorTree", "Global data collection");
}

G4bool GateGlobalActor::SkipThisStep(const G4Step* step)
{
	for(std::map<G4String, CheckFunction>::iterator checkIt = mCheckFunctionsPointersList.begin(); checkIt != mCheckFunctionsPointersList.end(); ++checkIt)
		if(!((this->*(checkIt->second))(*step)))
			return true;
	return false;
}

void GateGlobalActor::UpdateFromThisStep(const G4Step* step)
{
	for(std::map<G4String, UpdateMethod>::iterator updateIt = mUpdateMethodsPointersList.begin(); updateIt != mUpdateMethodsPointersList.end(); ++updateIt)
		(this->*(updateIt->second))(*step);
}

void GateGlobalActor::FillTree()
{
	if(pTree == nullptr)
		return;
	pTree->Fill();
}

void GateGlobalActor::TryAddUpdateMethod(const G4String& update_method_name, const UpdateMethod& update_method)
{
	assert(update_method_name.size() > 0);
	std::map<G4String, UpdateMethod>::iterator found = mUpdateMethodsPointersList.find(update_method_name);
	if(found == mUpdateMethodsPointersList.end())
		mUpdateMethodsPointersList.emplace(update_method_name, update_method);
}
template <class T>
void GateGlobalActor::TryAddBranch(const G4String& branch_name, T& variable)
{
	//Branch name must be setted
	assert(branch_name.size() > 0);

	if(pFile == nullptr)
	{
		InitFile(mFileName);
		InitTree();
	}

	if(pTree->FindBranch(branch_name.c_str()) == nullptr)
		pTree->Branch(branch_name.c_str(), &variable);
}

void GateGlobalActor::TryAddCheckFunction(const G4String& check_function_name, const CheckFunction& check_function)
{
	//Function name must be setted
	assert(check_function_name.size() > 0);

	std::map<G4String, CheckFunction>::iterator found = mCheckFunctionsPointersList.find(check_function_name);
	if(found == mCheckFunctionsPointersList.end())
		mCheckFunctionsPointersList.emplace(check_function_name, check_function);
}

template <class T>
void GateGlobalActor::TryAddToSet(std::set<T>& set, const T& value_to_add)
{
	if(set.find(value_to_add) == set.cend())
			set.insert(value_to_add);
}

G4double GateGlobalActor::deg(const G4double& angle_radians) const
{
	return (180.0/M_PI) * angle_radians;
}

G4double GateGlobalActor::keV(const G4double& energy_MeV) const
{
	return energy_MeV * 1000.0; //Because Geant4 (and that's why Gate too) work with MeV
}

void GateGlobalActor::ConvertToTVector3(const G4ThreeVector& from, TVector3& to)
{
	to = TVector3(from.x(), from.y(), from.z());
}

/******************************************************************Add below you functions and methods**********************************************************************************************/
void GateGlobalActor::SetFilterProcessName(const G4String& process_name)
{
	TryAddCheckFunction("CheckProcessName", &GateGlobalActor::CheckProcessName);
	TryAddToSet(mFilterProcessesNames, process_name);
}

G4bool GateGlobalActor::CheckProcessName(const G4Step& step) const
{
	return mFilterProcessesNames.find(step.GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()) != mFilterProcessesNames.cend();
}

void GateGlobalActor::SetFilterParticleName(const G4String& particle_name)
{
	TryAddCheckFunction("CheckParticleName", &GateGlobalActor::CheckParticleName);
	TryAddToSet(mFilterParticleName, particle_name);
}

G4bool GateGlobalActor::CheckParticleName(const G4Step& step) const
{
	return mFilterParticleName.find(step.GetTrack()->GetDefinition()->GetParticleName()) != mFilterParticleName.cend();
}

void GateGlobalActor::SetFilerParticlePDGCode(const G4int& pdg_code)
{
	TryAddCheckFunction("CheckPDGCode", &GateGlobalActor::CheckPDGCode);
	TryAddToSet(mFilterPDGCodes, pdg_code);
}

G4bool GateGlobalActor::CheckPDGCode(const G4Step& step) const
{
	return mFilterPDGCodes.find(step.GetTrack()->GetDefinition()->GetPDGEncoding()) != mFilterPDGCodes.cend();
}

void GateGlobalActor::SetFilerProcessAngle(const G4double& angle)
{
	TryAddCheckFunction("CheckProcessAngle", &GateGlobalActor::CheckProcessAngle);
	mFilterProcessAngle = angle;
}

G4bool GateGlobalActor::CheckProcessAngle(const G4Step& step) const
{
	G4double angle = (step.GetPreStepPoint()->GetMomentumDirection()).angle(step.GetPostStepPoint()->GetMomentumDirection());
	angle *= 180.0/M_PI;
	if(std::fabs(angle - mFilterProcessAngle) < pow(10,-3))
		return true;
	return false;
}

void GateGlobalActor::SetFilterEmissionPoint(const G4ThreeVector& emission_point)
{
	TryAddCheckFunction("CheckEmissionPoint", &GateGlobalActor::CheckEmissionPoint);
	mFilterEmissionPoint = emission_point;
}

G4bool GateGlobalActor::CheckEmissionPoint(const G4Step& step) const
{
	G4ThreeVector distance = step.GetTrack()->GetVertexPosition() - mFilterEmissionPoint;
	if(distance.dot(distance) < pow(10,-6))
		return true;
	return false;
}

void GateGlobalActor::SetEnableVolumeName()
{
	TryAddBranch("VolumeName", mVolumeName);
}

void GateGlobalActor::SetEnableScintilatorPosition()
{
	TryAddUpdateMethod("UpdateScintilatorPosition", &GateGlobalActor::UpdateScintilatorPosition);
	TryAddBranch("ScintilatorPosition", mScintilatorPosition);
}

void GateGlobalActor::UpdateScintilatorPosition(const G4Step& step)
{
	const G4TouchableHistory* touchable = dynamic_cast<const G4TouchableHistory*>(step.GetPostStepPoint()->GetTouchable());
	ConvertToTVector3(touchable->GetTranslation(), mScintilatorPosition);
}

void GateGlobalActor::SetEnableEventID()
{
	TryAddUpdateMethod("UpdateEventID", &GateGlobalActor::UpdateEventID);
	TryAddBranch("EventID", mEventID);
}

void GateGlobalActor::UpdateEventID(const G4Step&)
{
	mEventID = GateRunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
}

void GateGlobalActor::SetEnableTrackID()
{
	TryAddUpdateMethod("UpdateTrackID", &GateGlobalActor::UpdateTrackID);
	TryAddBranch("TrackID", mTrackID);
}

void GateGlobalActor::UpdateTrackID(const G4Step& step)
{
	mTrackID = step.GetTrack()->GetTrackID();
}

void GateGlobalActor::SetEnableEnergyBeforeProcess()
{
	TryAddUpdateMethod("UpdateEnergyBeforeProcess", &GateGlobalActor::UpdateEnergyBeforeProcess);
	TryAddBranch("EnergyBeforeProcess", mEnergyBeforeProcess);
}

void GateGlobalActor::UpdateEnergyBeforeProcess(const G4Step& step)
{
	mEnergyBeforeProcess = keV(step.GetPreStepPoint()->GetTotalEnergy());
}

void GateGlobalActor::SetEnableEnergyAfterProcess()
{
	TryAddUpdateMethod("UpdateEnergyAfterProcess", &GateGlobalActor::UpdateEnergyAfterProcess);
	TryAddBranch("EnergyAfterProcess", mEnergyAfterProcess);
}

void GateGlobalActor::UpdateEnergyAfterProcess(const G4Step& step)
{
	mEnergyAfterProcess = keV(step.GetPostStepPoint()->GetTotalEnergy());
}

void GateGlobalActor::SetEnableEnergyLossDuringProcess()
{
	TryAddUpdateMethod("UpdateEnergyLossDuringProcess", &GateGlobalActor::UpdateEnergyLossDuringProcess);
	TryAddBranch("EnergyLossDuringProcess", mEnergyLossDuringProcess);
}

void GateGlobalActor::UpdateEnergyLossDuringProcess(const G4Step& step)
{
	mEnergyLossDuringProcess = keV(step.GetPreStepPoint()->GetTotalEnergy() - step.GetPostStepPoint()->GetTotalEnergy());
}

void GateGlobalActor::SetEnableMomentumDirectionBeforeProcess()
{
	TryAddUpdateMethod("UpdateMomentumDirectionBeforeProcess", &GateGlobalActor::UpdateMomentumDirectionBeforeProcess);
	TryAddBranch("MomentumDirectionBeforeProcess", mMomentumDirectionBeforeProcess);
}

void GateGlobalActor::UpdateMomentumDirectionBeforeProcess(const G4Step& step)
{
	ConvertToTVector3(step.GetPreStepPoint()->GetMomentumDirection().unit(), mMomentumDirectionBeforeProcess);
}

void GateGlobalActor::SetEnableMomentumDirectionAfterProcess()
{
	TryAddUpdateMethod("UpdateMomentumDirectionAfterProcess", &GateGlobalActor::UpdateMomentumDirectionAfterProcess);
	TryAddBranch("MomentumDirectionAfterProcess", mMomentumDirectionAfterProcess);
}

void GateGlobalActor::UpdateMomentumDirectionAfterProcess(const G4Step& step)
{
	ConvertToTVector3(step.GetPostStepPoint()->GetMomentumDirection().unit(), mMomentumDirectionAfterProcess);
}

void GateGlobalActor::SetEnableProcessPosition()
{
	TryAddUpdateMethod("UpdateProcessPosition", &GateGlobalActor::UpdateProcessPosition);
	TryAddBranch("ProcessPosition", mProcessPosition);
}

void GateGlobalActor::UpdateProcessPosition(const G4Step& step)
{
	ConvertToTVector3(step.GetTrack()->GetPosition(), mProcessPosition);
}

void GateGlobalActor::SetEnableEmissionPointFromSource()
{
	TryAddUpdateMethod("UpdateEmissionPointFromSource", &GateGlobalActor::UpdateEmissionPointFromSource);
	TryAddBranch("EmissionPointFromSource", mEmissionPointFromSource);
}

void GateGlobalActor::UpdateEmissionPointFromSource(const G4Step& step)
{
	ConvertToTVector3(step.GetTrack()->GetVertexPosition(), mEmissionPointFromSource);
}

void GateGlobalActor::SetEnableEmissionMomentumDirectionFromSource()
{
	TryAddUpdateMethod("UpdateEmissionMomentumDirectionFromSource", &GateGlobalActor::UpdateEmissionMomentumDirectionFromSource);
	TryAddBranch("EmissionMomentumDirectionFromSource", mEmissionMomentumDirectionFromSource);
}

void GateGlobalActor::UpdateEmissionMomentumDirectionFromSource(const G4Step& step)
{
	ConvertToTVector3(step.GetTrack()->GetVertexMomentumDirection(), mEmissionMomentumDirectionFromSource);
}

void GateGlobalActor::SetEnableEmissionEnergyFromSource()
{
	TryAddUpdateMethod("UpdateEmissionEnergyFromSource", &GateGlobalActor::UpdateEmissionEnergyFromSource);
	TryAddBranch("EmissionEnergyFromSource", mEmissionEnergyFromSource);
}

void GateGlobalActor::UpdateEmissionEnergyFromSource(const G4Step& step)
{
	mEmissionEnergyFromSource = keV(step.GetTrack()->GetVertexKineticEnergy());
}

void GateGlobalActor::SetEnableParticleName()
{
	TryAddUpdateMethod("UpdateParticleName", &GateGlobalActor::UpdateParticleName);
	TryAddBranch("ParticleName", mParticleName);
}

void GateGlobalActor::UpdateParticleName(const G4Step& step)
{
	mParticleName = step.GetTrack()->GetDefinition()->GetParticleName();
}

void GateGlobalActor::SetEnableParticlePGDCoding()
{
	TryAddUpdateMethod("UpdateParticlePGDCoding", &GateGlobalActor::UpdateParticlePGDCoding);
	TryAddBranch("ParticlePGDCoding", mParticlePGDCoding);
}

void GateGlobalActor::UpdateParticlePGDCoding(const G4Step& step)
{
	mParticlePGDCoding = step.GetTrack()->GetDefinition()->GetPDGEncoding();
}

void GateGlobalActor::SetEnableProcessAngle()
{
	TryAddUpdateMethod("UpdateProcessAngle", &GateGlobalActor::UpdateProcessAngle);
	TryAddBranch("ProcessAngle", mProcessAngle);
}

void GateGlobalActor::UpdateProcessAngle(const G4Step& step)
{
	G4ThreeVector mom_dir1= step.GetPreStepPoint()->GetMomentumDirection().unit();
	G4ThreeVector mom_dir2= step.GetPostStepPoint()->GetMomentumDirection().unit();
	mProcessAngle = mom_dir1.angle(mom_dir2);
}

void GateGlobalActor::SetEnablePolarizationBeforeProcess()
{
	TryAddUpdateMethod("UpdatePolarizationBeforeProcess", &GateGlobalActor::UpdatePolarizationBeforeProcess);
	TryAddBranch("PolarizationBeforeProcess", mPolarizationBeforeProcess);
}

void GateGlobalActor::UpdatePolarizationBeforeProcess(const G4Step& step)
{
	ConvertToTVector3(step.GetPreStepPoint()->GetPolarization().unit(), mPolarizationBeforeProcess);
}

void GateGlobalActor::SetEnablePolarizationAfterProcess()
{
	TryAddUpdateMethod("UpdatePolarizationAfterProcess", &GateGlobalActor::UpdatePolarizationAfterProcess);
	TryAddBranch("PolarizationAfterProcess", mPolarizationAfterProcess);
}

void GateGlobalActor::UpdatePolarizationAfterProcess(const G4Step& step)
{
	ConvertToTVector3(step.GetPostStepPoint()->GetPolarization().unit(), mPolarizationAfterProcess);
}

void GateGlobalActor::SetEnableProcessName()
{
	TryAddUpdateMethod("UpdateProcessName", &GateGlobalActor::UpdateProcessName);
	TryAddBranch("ProcessName", mProcessName);
}

void GateGlobalActor::UpdateProcessName(const G4Step& step)
{
	mProcessName = step.GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
}
