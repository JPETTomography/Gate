/**
 *  @copyright Copyright 2019 The J-PET Gate Authors. All rights reserved.
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
 *  @file GateGlobalActorHit.cc
 */

#include "GateGlobalActorHit.hh"
#include "G4VProcess.hh"
#include "GateRunManager.hh"
#include "G4Track.hh"
#include <GateGammaModelPrimaryParticleInformation.hh>

GateGlobalActorHit::GateGlobalActorHit() {}

GateGlobalActorHit::~GateGlobalActorHit() {}

GateGlobalActorHit::GateGlobalActorHit( const G4Step& step, const std::string& volume_name ) 
{
 mVolumeName = volume_name;
 extractDataFromStep( step );
}

void GateGlobalActorHit::setVolumeName( const std::string& volume_name ) { mVolumeName = volume_name; }

std::string GateGlobalActorHit::getVolumeName() const { return mVolumeName; }

void GateGlobalActorHit::setScintillatorPosition( const TVector3& position ) { mScintillatorPosition = position; }

TVector3 GateGlobalActorHit::getScintillatorPosition() const { return mScintillatorPosition; }

void GateGlobalActorHit::setEventID( const G4int& event_id ) { mEventID = event_id; }

G4int GateGlobalActorHit::getEventID() const { return mEventID; }

void GateGlobalActorHit::setTrackID( const G4int& track_id ) { mTrackID = track_id; }

G4int GateGlobalActorHit::getTrackID() const { return mTrackID; }

void GateGlobalActorHit::setEnergyBeforeProcess( const G4double& energy ) { mEnergyBeforeProcess = energy; }

G4double GateGlobalActorHit::getEnergyBeforeProcess() const { return mEnergyBeforeProcess; }

void GateGlobalActorHit::setEnergyAfterProcess( const G4double& energy ) { mEnergyAfterProcess = energy; }

G4double GateGlobalActorHit::getEnergyAfterProcess() const { return mEnergyAfterProcess; }

void GateGlobalActorHit::setEnergyLossDuringProcess( const G4double& energy ) { mEnergyLossDuringProcess = energy; }

G4double GateGlobalActorHit::getEnergyLossDuringProcess() const { return mEnergyLossDuringProcess; }

void GateGlobalActorHit::setMomentumDirectionBeforeProcess( const TVector3& momentum ) { mMomentumDirectionBeforeProcess = momentum; }

TVector3 GateGlobalActorHit::getMomentumDirectionBeforeProcess() const { return mMomentumDirectionBeforeProcess; }

void GateGlobalActorHit::setMomentumDirectionAfterProcess( const TVector3& momentum ) { mMomentumDirectionAfterProcess = momentum; }

TVector3 GateGlobalActorHit::getMomentumDirectionAfterProcess() const { return mMomentumDirectionAfterProcess; }

void GateGlobalActorHit::setProcessPosition( const TVector3& position) { mProcessPosition = position; }

TVector3 GateGlobalActorHit::getProcessPosition() const { return mProcessPosition; }

void GateGlobalActorHit::setEmissionPointFromSource( const TVector3& position) { mEmissionPointFromSource = position; }

TVector3 GateGlobalActorHit::getEmissionPointFromSource() const { return mEmissionPointFromSource; }

void GateGlobalActorHit::setEmissionMomentumDirectionFromSource( const TVector3& momentum ) { mEmissionMomentumDirectionFromSource = momentum; }

TVector3 GateGlobalActorHit::getEmissionMomentumDirectionFromSource() const { return mEmissionMomentumDirectionFromSource; }

void GateGlobalActorHit::setEmissionEnergyFromSource( const G4double& energy ) { mEmissionEnergyFromSource = energy; }

G4double GateGlobalActorHit::getEmissionEnergyFromSource() const { return mEmissionEnergyFromSource; }

void GateGlobalActorHit::setParticleName( const std::string& particle_name ) { mParticleName = particle_name; }

std::string GateGlobalActorHit::getParticleName() const { return mParticleName; }

void GateGlobalActorHit::setParticlePGDCoding( const G4int& pgd_code ) { mParticlePGDCoding = pgd_code; }

G4int GateGlobalActorHit::getParticlePGDCoding() const { return mParticlePGDCoding; }

void GateGlobalActorHit::setPolarizationBeforeProcess( const TVector3& polarization ) { mPolarizationBeforeProcess = polarization; }

TVector3 GateGlobalActorHit::getPolarizationBeforeProcess() const { return mPolarizationBeforeProcess; }

void GateGlobalActorHit::setPolarizationAfterProcess( const TVector3& polarization ) { mPolarizationAfterProcess = polarization; }

TVector3 GateGlobalActorHit::getPolarizationAfterProcess() const { return mPolarizationAfterProcess; }

void GateGlobalActorHit::setProcessName( const std::string& process_name ) { mProcessName = process_name; }

std::string GateGlobalActorHit::getProcessName() const { return mProcessName; }

void GateGlobalActorHit::setParentID( const G4int& parent_id ) { mParentID = parent_id; }

G4int GateGlobalActorHit::getParentID() const { return mParentID; }

void GateGlobalActorHit::setInteractionTime( const G4double& time ) { mInteractionTime = time; }

G4double GateGlobalActorHit::getInteractionTime() const { return mInteractionTime; }

void GateGlobalActorHit::setLocalTime( const G4double& time ) { mLocalTime = time; }

G4double GateGlobalActorHit::getLocalTime() const { return mLocalTime; }

void GateGlobalActorHit::setGlobalTime( const G4double& time ) { mGlobalTime = time; }

G4double GateGlobalActorHit::getGlobalTime() const { return mGlobalTime; }

void GateGlobalActorHit::setProperTime( const G4double& time ) { mProperTime = time; }

G4double GateGlobalActorHit::getProperTime() const { return mProperTime; }

void GateGlobalActorHit::setMultipleScatteringNumber( const G4int& multiplicity ) { mMultipleScatteringNumber = multiplicity; }

G4int GateGlobalActorHit::getMultipleScatteringNumber() const { return mMultipleScatteringNumber; }

void GateGlobalActorHit::setScatteringIndex( const unsigned int& index ) { mScatteringIndex = index; }
 
G4int GateGlobalActorHit::getScatteringIndex() const { return mScatteringIndex; }

G4int GateGlobalActorHit::getGammaSourceModel() const { return mGammaSourceModel; }
  
G4int GateGlobalActorHit::getGammaKind() const { return mGammaKind; }

TVector3 GateGlobalActorHit::getInitialPolarization() const { return mInitialPolarization; }

bool GateGlobalActorHit::isTheSameScintillator( const GateGlobalActorHit& hit ) const { return  mScintillatorPosition == hit.mScintillatorPosition; }

bool GateGlobalActorHit::isTheSameEventID( const GateGlobalActorHit& hit ) const { return mEventID == hit.mEventID; }

bool GateGlobalActorHit::isTheSameTrackID( const GateGlobalActorHit& hit ) const { return mTrackID == hit.mTrackID; }

void GateGlobalActorHit::extractDataFromStep( const G4Step& step )
{
 const G4TouchableHistory* touchable = dynamic_cast<const G4TouchableHistory*>(step.GetPreStepPoint()->GetTouchable());
 mScintillatorPosition = getTVector3( touchable->GetTranslation() );

 mEventID = GateRunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

 mTrackID = step.GetTrack()->GetTrackID();

 mEnergyBeforeProcess = keV( step.GetPreStepPoint()->GetTotalEnergy() );

 mEnergyAfterProcess = keV( step.GetPostStepPoint()->GetTotalEnergy() );

 mEnergyLossDuringProcess = keV( step.GetTotalEnergyDeposit() );

 mMomentumDirectionBeforeProcess = getTVector3( step.GetPreStepPoint()->GetMomentumDirection() );

 mMomentumDirectionAfterProcess = getTVector3( step.GetPostStepPoint()->GetMomentumDirection() );

 mProcessPosition = getTVector3( step.GetPostStepPoint()->GetPosition() );

 mEmissionPointFromSource = getTVector3( step.GetTrack()->GetVertexPosition() );

 mEmissionMomentumDirectionFromSource = getTVector3( step.GetTrack()->GetVertexMomentumDirection() );

 mEmissionEnergyFromSource = keV( step.GetTrack()->GetVertexKineticEnergy() );

 mParticleName = step.GetTrack()->GetParticleDefinition()->GetParticleName(); 

 mParticlePGDCoding = step.GetTrack()->GetParticleDefinition()->GetPDGEncoding();

 mPolarizationBeforeProcess = getTVector3( step.GetPreStepPoint()->GetPolarization() );

 mPolarizationAfterProcess = getTVector3( step.GetPostStepPoint()->GetPolarization() );

 mProcessName = step.GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

 mParentID = step.GetTrack()->GetParentID();

 mInteractionTime = step.GetDeltaTime();

 mLocalTime = step.GetTrack()->GetLocalTime();

 mGlobalTime = step.GetPostStepPoint()->GetGlobalTime();

 mProperTime = step.GetTrack()->GetProperTime();

 GateGammaModelPrimaryParticleInformation* info = dynamic_cast<GateGammaModelPrimaryParticleInformation*>( step.GetTrack()->GetDynamicParticle()->GetPrimaryParticle()->GetUserInformation() );

 mGammaSourceModel = info->getGammaSourceModel();

 mGammaKind = info->getGammaKind();

 mInitialPolarization = getTVector3( info->getInitialPolarization() );
 
}

TVector3 GateGlobalActorHit::getTVector3( const G4ThreeVector& input_vector ) const
{
 TVector3 output_vector( input_vector.x(), input_vector.y(), input_vector.z() );
 return output_vector;
}

G4double GateGlobalActorHit::keV( const G4double& energy ) const
{
 return energy * 1000.0; //Because Geant4 (and that's why Gate too) work with MeV
}
