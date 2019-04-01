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
#include<iostream>

std::unique_ptr<GateGlobalActor> GateGlobalActor::upInstance;

GateGlobalActor::GateGlobalActor() {}

GateGlobalActor::~GateGlobalActor() 
{ 
 std::cout << "GateGlobalActor::~GateGlobalActor()" << std::endl;
 saveHitsFromAdder();
 Write();
}

GateGlobalActor* GateGlobalActor::Instance()
{
 if( upInstance.get() == nullptr ) { upInstance.reset( new GateGlobalActor() ); }
 return upInstance.get();
}

void GateGlobalActor::Write()
{
 assert( pFile != nullptr);
 assert( pTree != nullptr );

 pFile = pTree->GetCurrentFile();
 pFile->Write();
}

void GateGlobalActor::Reset()
{
 assert( pTree != nullptr );
 pTree->Reset();
}

void GateGlobalActor::CloseActor()
{
 saveHitsFromAdder();
 Write();
 pFile = nullptr;
 pTree = nullptr;
 mUseAdder = false;
 mCheckFunctionsPointersList.clear();
 mUpdateMethodsPointersList.clear();
 mAdder.reset();
}

void GateGlobalActor::saveHitsFromAdder()
{
   GateGlobalActorHits hits =  mAdder.getHits();
   for ( GateGlobalActorHits::const_iterator ihit = hits.cbegin(); ihit != hits.cend(); ++ihit ) {  UpdateFromThisHit( *ihit ); }
   mAdder.reset();
}

void GateGlobalActor::NoticeStep( const G4String& volume_name, const G4Step* step )
{
 GateGlobalActorHit hit( *step, volume_name );

 if ( mUseAdder )
 {
  if ( mAdder.isReadyToMerge( hit ) ) { saveHitsFromAdder(); } 
  if ( !SkipThisHit( hit ) ) { mAdder.processHit( hit ); }
 }
 else if ( !SkipThisHit( hit ) ) { UpdateFromThisHit( hit ); }
}

void GateGlobalActor::SetFileName( const G4String& file_name )
{
 assert( file_name.size() > 0 );
 if( file_name.rfind( ".root" ) == std::string::npos ) { mFileName = file_name + ".root"; }
 else { mFileName = file_name; }
}

void GateGlobalActor::InitFile( const G4String& file_name )
{
 assert( file_name.size() > 0 );
 assert( pFile == nullptr );

 if( file_name.rfind( ".root" ) == std::string::npos ) { pFile = new TFile( ( file_name + std::string( ".root" ) ).c_str(), "RECREATE" ); }
 else { pFile = new TFile( file_name.c_str(), "RECREATE" ); }
}

void GateGlobalActor::GateGlobalActor::InitTree()
{
 assert( pTree == nullptr );
 pTree = new TTree( "GateGlobalActorTree", "Global data collection" );
}

G4bool GateGlobalActor::SkipThisHit( const GateGlobalActorHit& hit )
{
 for( std::map<G4String, CheckFunction>::iterator checkIt = mCheckFunctionsPointersList.begin(); checkIt != mCheckFunctionsPointersList.end(); ++checkIt )
 {
  if( !( ( this->*( checkIt->second ) )( hit ) ) ) { return true; }
 }
 return false;
}

void GateGlobalActor::UpdateFromThisHit( const GateGlobalActorHit& hit )
{
 for (std::map<G4String, UpdateMethod>::iterator updateIt = mUpdateMethodsPointersList.begin(); updateIt != mUpdateMethodsPointersList.end(); ++updateIt )
 {
  ( this->*( updateIt->second ) )( hit );
 }
 FillTree();
}

void GateGlobalActor::FillTree()
{
 assert( pTree != nullptr );
 pTree->Fill();
}

void GateGlobalActor::TryAddUpdateMethod( const G4String& update_method_name, const UpdateMethod& update_method )
{
 assert( update_method_name.size() > 0 );
 std::map<G4String, UpdateMethod>::iterator found = mUpdateMethodsPointersList.find( update_method_name );
 if( found == mUpdateMethodsPointersList.end() ) { mUpdateMethodsPointersList.emplace( update_method_name, update_method ); }
}

template <class T>
void GateGlobalActor::TryAddBranch( const G4String& branch_name, T& variable )
{
 //Branch name must be setted
 assert( branch_name.size() > 0 );

 if( pFile == nullptr )
 {
  InitFile( mFileName );
  InitTree();
 }

 assert( pTree != nullptr );
 if( pTree->FindBranch( branch_name.c_str() ) == nullptr ) { pTree->Branch(branch_name.c_str(), &variable); }
}

void GateGlobalActor::TryAddCheckFunction( const G4String& check_function_name, const CheckFunction& check_function )
{
 //Function name must be setted
 assert( check_function_name.size() > 0 );

 std::map<G4String, CheckFunction>::iterator found = mCheckFunctionsPointersList.find( check_function_name );
 if ( found == mCheckFunctionsPointersList.end() ) { mCheckFunctionsPointersList.emplace( check_function_name, check_function ); }
}

template <class T>
void GateGlobalActor::TryAddToSet( std::set<T>& set, const T& value_to_add )
{
 if ( set.find( value_to_add ) == set.cend() ) { set.insert( value_to_add ); }
}

void GateGlobalActor::SetEnableAdder() { mUseAdder = true; }

void GateGlobalActor::SetTimeIntervalBetweenHits( const G4double& time ) { mAdder.setTimeIntervalBetweenHits( time ); }

/******************************************************************Add below you functions and methods**********************************************************************************************/

void GateGlobalActor::SetFilterProcessName( const G4String& process_name )
{
 TryAddCheckFunction( "CheckProcessName", &GateGlobalActor::CheckProcessName );
 TryAddToSet( mFilterProcessesNames, process_name );
}

G4bool GateGlobalActor::CheckProcessName( const GateGlobalActorHit& hit ) const
{
 return mFilterProcessesNames.find( hit.getProcessName() ) != mFilterProcessesNames.cend();
}

void GateGlobalActor::SetFilterParticleName( const G4String& particle_name )
{
 TryAddCheckFunction( "CheckParticleName", &GateGlobalActor::CheckParticleName );
 TryAddToSet( mFilterParticleName, particle_name );
}

G4bool GateGlobalActor::CheckParticleName( const GateGlobalActorHit& hit ) const
{
 return mFilterParticleName.find( hit.getParticleName() ) != mFilterParticleName.cend();
}

void GateGlobalActor::SetFilerParticlePDGCode( const G4int& pdg_code )
{
 TryAddCheckFunction( "CheckPDGCode", &GateGlobalActor::CheckPDGCode );
 TryAddToSet( mFilterPDGCodes, pdg_code );
}

G4bool GateGlobalActor::CheckPDGCode( const GateGlobalActorHit& hit ) const
{
 return mFilterPDGCodes.find( hit.getParticlePGDCoding() ) != mFilterPDGCodes.cend();
}

void GateGlobalActor::SetFilerProcessAngle( const G4double& angle )
{
 TryAddCheckFunction( "CheckProcessAngle", &GateGlobalActor::CheckProcessAngle );
 mFilterProcessAngle = angle;
}

G4bool GateGlobalActor::CheckProcessAngle( const GateGlobalActorHit& hit ) const
{
 G4double angle = ( hit.getMomentumDirectionBeforeProcess() ).Angle( hit.getMomentumDirectionAfterProcess() ) * TMath::RadToDeg();
 if ( TMath::Abs( angle - mFilterProcessAngle ) < TMath::Power( 10.0 , -3.0) ) { return true; }
 return false;
}

void GateGlobalActor::SetFilterEmissionPoint( const G4ThreeVector& emission_point )
{
 TryAddCheckFunction( "CheckEmissionPoint", &GateGlobalActor::CheckEmissionPoint );
 mFilterEmissionPoint = TVector3( emission_point.x(), emission_point.y(), emission_point.z() );
}

G4bool GateGlobalActor::CheckEmissionPoint( const GateGlobalActorHit& hit ) const
{
 TVector3 distance = hit.getEmissionPointFromSource() - mFilterEmissionPoint;
 if ( distance.Mag() < TMath::Power( 10.0 , -6.0) ) { return true; }
 return false;
}

void GateGlobalActor::SetEnableVolumeName()
{
 TryAddUpdateMethod( "UpdateVolumeName", &GateGlobalActor::UpdateVolumeName );
 TryAddBranch( "VolumeName", mVolumeName );
}

void GateGlobalActor::UpdateVolumeName( const GateGlobalActorHit& hit ) { mVolumeName = hit.getVolumeName(); }

void GateGlobalActor::SetEnableScintilatorPosition()
{
 TryAddUpdateMethod( "UpdateScintilatorPosition", &GateGlobalActor::UpdateScintilatorPosition );
 TryAddBranch( "ScintilatorPosition", mScintilatorPosition );
}

void GateGlobalActor::UpdateScintilatorPosition( const GateGlobalActorHit& hit ) { mScintilatorPosition = hit.getScintillatorPosition(); }

void GateGlobalActor::SetEnableEventID()
{
 TryAddUpdateMethod( "UpdateEventID", &GateGlobalActor::UpdateEventID );
 TryAddBranch( "EventID", mEventID );
}

void GateGlobalActor::UpdateEventID( const GateGlobalActorHit& hit ) { mEventID = hit.getEventID(); }

void GateGlobalActor::SetEnableTrackID()
{
 TryAddUpdateMethod( "UpdateTrackID", &GateGlobalActor::UpdateTrackID );
 TryAddBranch( "TrackID", mTrackID );
}

void GateGlobalActor::UpdateTrackID( const GateGlobalActorHit& hit ) { mTrackID = hit.getTrackID(); }

void GateGlobalActor::SetEnableEnergyBeforeProcess()
{
 TryAddUpdateMethod( "UpdateEnergyBeforeProcess", &GateGlobalActor::UpdateEnergyBeforeProcess );
 TryAddBranch( "EnergyBeforeProcess", mEnergyBeforeProcess );
}

void GateGlobalActor::UpdateEnergyBeforeProcess( const GateGlobalActorHit& hit ) { mEnergyBeforeProcess = hit.getEnergyBeforeProcess(); }

void GateGlobalActor::SetEnableEnergyAfterProcess()
{
 TryAddUpdateMethod( "UpdateEnergyAfterProcess", &GateGlobalActor::UpdateEnergyAfterProcess );
 TryAddBranch( "EnergyAfterProcess", mEnergyAfterProcess );
}

void GateGlobalActor::UpdateEnergyAfterProcess( const GateGlobalActorHit& hit ) { mEnergyAfterProcess = hit.getEnergyAfterProcess(); }

void GateGlobalActor::SetEnableEnergyLossDuringProcess()
{
 TryAddUpdateMethod( "UpdateEnergyLossDuringProcess", &GateGlobalActor::UpdateEnergyLossDuringProcess );
 TryAddBranch( "EnergyLossDuringProcess", mEnergyLossDuringProcess );
}

void GateGlobalActor::UpdateEnergyLossDuringProcess( const GateGlobalActorHit& hit ) { mEnergyLossDuringProcess = hit.getEnergyLossDuringProcess(); }

void GateGlobalActor::SetEnableMomentumDirectionBeforeProcess()
{
 TryAddUpdateMethod( "UpdateMomentumDirectionBeforeProcess", &GateGlobalActor::UpdateMomentumDirectionBeforeProcess );
 TryAddBranch( "MomentumDirectionBeforeProcess", mMomentumDirectionBeforeProcess );
}

void GateGlobalActor::UpdateMomentumDirectionBeforeProcess( const GateGlobalActorHit& hit ) { mMomentumDirectionBeforeProcess = hit.getMomentumDirectionBeforeProcess(); }

void GateGlobalActor::SetEnableMomentumDirectionAfterProcess()
{
 TryAddUpdateMethod( "UpdateMomentumDirectionAfterProcess", &GateGlobalActor::UpdateMomentumDirectionAfterProcess );
 TryAddBranch( "MomentumDirectionAfterProcess", mMomentumDirectionAfterProcess );
}

void GateGlobalActor::UpdateMomentumDirectionAfterProcess(const GateGlobalActorHit& hit ) { mMomentumDirectionAfterProcess = hit.getMomentumDirectionAfterProcess(); }


void GateGlobalActor::SetEnableProcessPosition()
{
 TryAddUpdateMethod( "UpdateProcessPosition", &GateGlobalActor::UpdateProcessPosition );
 TryAddBranch( "ProcessPosition", mProcessPosition );
}

void GateGlobalActor::UpdateProcessPosition( const GateGlobalActorHit& hit ) { mProcessPosition = hit.getProcessPosition(); }

void GateGlobalActor::SetEnableEmissionPointFromSource()
{
 TryAddUpdateMethod( "UpdateEmissionPointFromSource", &GateGlobalActor::UpdateEmissionPointFromSource );
 TryAddBranch( "EmissionPointFromSource", mEmissionPointFromSource );
}

void GateGlobalActor::UpdateEmissionPointFromSource( const GateGlobalActorHit& hit ) { mEmissionPointFromSource = hit.getEmissionPointFromSource(); }

void GateGlobalActor::SetEnableEmissionMomentumDirectionFromSource()
{
 TryAddUpdateMethod( "UpdateEmissionMomentumDirectionFromSource", &GateGlobalActor::UpdateEmissionMomentumDirectionFromSource );
 TryAddBranch( "EmissionMomentumDirectionFromSource", mEmissionMomentumDirectionFromSource );
}

void GateGlobalActor::UpdateEmissionMomentumDirectionFromSource( const GateGlobalActorHit& hit ) { mEmissionMomentumDirectionFromSource = hit.getEmissionMomentumDirectionFromSource(); }

void GateGlobalActor::SetEnableEmissionEnergyFromSource()
{
 TryAddUpdateMethod( "UpdateEmissionEnergyFromSource", &GateGlobalActor::UpdateEmissionEnergyFromSource );
 TryAddBranch( "EmissionEnergyFromSource", mEmissionEnergyFromSource );
}

void GateGlobalActor::UpdateEmissionEnergyFromSource( const GateGlobalActorHit& hit ) { mEmissionEnergyFromSource = hit.getEmissionEnergyFromSource(); }

void GateGlobalActor::SetEnableParticleName()
{
 TryAddUpdateMethod( "UpdateParticleName", &GateGlobalActor::UpdateParticleName );
 TryAddBranch( "ParticleName", mParticleName );
}

void GateGlobalActor::UpdateParticleName( const GateGlobalActorHit& hit ) { mParticleName = hit.getParticleName(); }

void GateGlobalActor::SetEnableParticlePGDCoding()
{
 TryAddUpdateMethod( "UpdateParticlePGDCoding", &GateGlobalActor::UpdateParticlePGDCoding );
 TryAddBranch( "ParticlePGDCoding", mParticlePGDCoding );
}

void GateGlobalActor::UpdateParticlePGDCoding( const GateGlobalActorHit& hit ) { mParticlePGDCoding = hit.getParticlePGDCoding(); }

void GateGlobalActor::SetEnableProcessAngle()
{
 TryAddUpdateMethod( "UpdateProcessAngle", &GateGlobalActor::UpdateProcessAngle );
 TryAddBranch( "ProcessAngle", mProcessAngle );
}

void GateGlobalActor::UpdateProcessAngle( const GateGlobalActorHit& hit ) { mProcessAngle = hit.getMomentumDirectionBeforeProcess().Angle( hit.getMomentumDirectionAfterProcess() ) * TMath::RadToDeg(); }

void GateGlobalActor::SetEnablePolarizationBeforeProcess()
{
 TryAddUpdateMethod( "UpdatePolarizationBeforeProcess", &GateGlobalActor::UpdatePolarizationBeforeProcess );
 TryAddBranch( "PolarizationBeforeProcess", mPolarizationBeforeProcess );
}

void GateGlobalActor::UpdatePolarizationBeforeProcess( const GateGlobalActorHit& hit ) { mPolarizationBeforeProcess = hit.getPolarizationBeforeProcess(); }

void GateGlobalActor::SetEnablePolarizationAfterProcess()
{
 TryAddUpdateMethod( "UpdatePolarizationAfterProcess", &GateGlobalActor::UpdatePolarizationAfterProcess );
 TryAddBranch( "PolarizationAfterProcess", mPolarizationAfterProcess );
}

void GateGlobalActor::UpdatePolarizationAfterProcess( const GateGlobalActorHit& hit ) { mPolarizationAfterProcess = hit.getPolarizationAfterProcess(); }

void GateGlobalActor::SetEnableProcessName()
{
 TryAddUpdateMethod( "UpdateProcessName", &GateGlobalActor::UpdateProcessName );
 TryAddBranch( "ProcessName", mProcessName );
}

void GateGlobalActor::UpdateProcessName( const GateGlobalActorHit& hit ) { mProcessName = hit.getProcessName(); }

void GateGlobalActor::SetEnableParentID()
{
 TryAddUpdateMethod( "UpdateParentID", &GateGlobalActor::UpdateParentID );
 TryAddBranch( "ParentID", mParentID );
}

void GateGlobalActor::UpdateParentID( const GateGlobalActorHit& hit ) { mParentID = hit.getParentID(); }

void GateGlobalActor::SetEnableInteractionTime()
{
 TryAddUpdateMethod( "UpdateInteractionTime", &GateGlobalActor::UpdateInteractionTime );
 TryAddBranch( "InteractionTime", mInteractionTime );
}

void GateGlobalActor::UpdateInteractionTime( const GateGlobalActorHit& hit ) { mInteractionTime = hit.getInteractionTime(); }

void GateGlobalActor::SetEnableLocalTime()
{
 TryAddUpdateMethod( "UpdateLocalTime", &GateGlobalActor::UpdateLocalTime );
 TryAddBranch( "LocalTime", mLocalTime );
}

void GateGlobalActor::UpdateLocalTime( const GateGlobalActorHit& hit ) { mLocalTime = hit.getLocalTime(); }

void GateGlobalActor::SetEnableGlobalTime()
{
 TryAddUpdateMethod( "UpdateGlobalTime", &GateGlobalActor::UpdateGlobalTime );
 TryAddBranch( "GlobalTime", mGlobalTime );
}

void GateGlobalActor::UpdateGlobalTime( const GateGlobalActorHit& hit ) { mGlobalTime = hit.getGlobalTime(); }


void GateGlobalActor::SetEnableProperTime()
{
 TryAddUpdateMethod( "UpdateProperTime", &GateGlobalActor::UpdateProperTime );
 TryAddBranch( "ProperTime", mProperTime );
}

void GateGlobalActor::UpdateProperTime( const GateGlobalActorHit& hit ) { mProperTime = hit.getProperTime(); }

void GateGlobalActor::SetFilterIgnoreProcessName( const G4String& process_name )
{
 TryAddCheckFunction( "CheckIgnoreProcessName", &GateGlobalActor::CheckIgnoreProcessName );
 TryAddToSet( mFilterIgnoreProcessesNames, process_name );
}

G4bool GateGlobalActor::CheckIgnoreProcessName( const GateGlobalActorHit& hit ) const
{
 return !( mFilterIgnoreProcessesNames.find( hit.getProcessName() ) != mFilterIgnoreProcessesNames.cend() );
}
