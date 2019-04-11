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
 *  @file GateGlobalActorHit.hh
 */

#ifndef GateGlobalActorHit_h
#define GateGlobalActorHit_h

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include <string>
#include "TVector3.h"

/**
 * @About: This class represents a single hit in observed volume by Global Actor (GateGlobalAtor). This is format of date which is used by Global Actor and by Global Actor Adder.
 * Concept of class is to extract (useful for Global Actor) data from step (G4Step).
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
**/
class GateGlobalActorHit
{
 public:
  GateGlobalActorHit();
  GateGlobalActorHit( const G4Step& step, const std::string& volume_name );
  ~GateGlobalActorHit();

  //get and set methods

  void setVolumeName( const std::string& volume_name );
  std::string getVolumeName() const;

  void setScintillatorPosition( const TVector3& position );
  TVector3 getScintillatorPosition() const;

  void setEventID( const G4int& event_id );
  G4int getEventID() const;

  void setTrackID( const G4int& track_id );
  G4int getTrackID() const;

  void setEnergyBeforeProcess( const G4double& energy );
  G4double getEnergyBeforeProcess() const;

  void setEnergyAfterProcess( const G4double& energy );
  G4double getEnergyAfterProcess() const;

  void setEnergyLossDuringProcess( const G4double& energy );
  G4double getEnergyLossDuringProcess() const;

  void setMomentumDirectionBeforeProcess( const TVector3& momentum );
  TVector3 getMomentumDirectionBeforeProcess() const;

  void setMomentumDirectionAfterProcess( const TVector3& momentum );
  TVector3 getMomentumDirectionAfterProcess() const;

  void setProcessPosition( const TVector3& position);
  TVector3 getProcessPosition() const;

  void setEmissionPointFromSource( const TVector3& position);
  TVector3 getEmissionPointFromSource() const;

  void setEmissionMomentumDirectionFromSource( const TVector3& momentum );
  TVector3 getEmissionMomentumDirectionFromSource() const;

  void setEmissionEnergyFromSource( const G4double& energy );
  G4double getEmissionEnergyFromSource() const;

  void setParticleName( const std::string& particle_name );
  std::string getParticleName() const;

  void setParticlePGDCoding( const G4int& pgd_code );
  G4int getParticlePGDCoding() const;

  void setPolarizationBeforeProcess( const TVector3& polarization );
  TVector3 getPolarizationBeforeProcess() const;

  void setPolarizationAfterProcess( const TVector3& polarization );
  TVector3 getPolarizationAfterProcess() const;

  void setProcessName( const std::string& process_name );
  std::string getProcessName() const;

  void setParentID( const G4int& parent_id );
  G4int getParentID() const;

  void setInteractionTime( const G4double& time );
  G4double getInteractionTime() const;

  void setLocalTime( const G4double& time );
  G4double getLocalTime() const;

  void setGlobalTime( const G4double& time );
  G4double getGlobalTime() const;

  void setProperTime( const G4double& time );
  G4double getProperTime() const;  

  void setMultipleScatteringNumber( const G4int& multiplicity );
  G4int getMultipleScatteringNumber() const;

  void setScatteringIndex( const unsigned int& index );
  G4int getScatteringIndex() const;

  G4int getGammaSourceModel() const;
  
  G4int getGammaKind() const;

  TVector3 getInitialPolarization() const;

  void setIsMerged( bool merged );
  bool getIsMerged() const;

  // comparing methods
  bool isTheSameScintillator( const GateGlobalActorHit& hit ) const;
  bool isTheSameEventID( const GateGlobalActorHit& hit ) const;
  bool isTheSameTrackID( const GateGlobalActorHit& hit ) const;

 private:
  void extractDataFromStep( const G4Step& step );
  TVector3 getTVector3( const G4ThreeVector& input_vector ) const;
  G4double keV( const G4double& energy ) const;

 private:
 //because this value is always positive in correct situation we can use negative value as indicator of improper initialization
 /** About initialized values for listed below variables:
  * variables with type G4int are always posiive in correct situation so we can use negative value as indicator of improper initialization
  * std::string - should always have not empty string, so empty string indicates improper initialization
  * TVector3 - mayor part of variables can't have zero vector, so zero vector indicates improper initialization
  * G4double - only "Transposition" process has energy deposition equal zero, so zero value indicates improper initialization for other variables
 **/

  std::string mVolumeName = "";

  TVector3 mScintillatorPosition = TVector3( 0, 0, 0);

  G4int mEventID = -1;

  G4int mTrackID = -1;

  G4double mEnergyBeforeProcess = 0.0;

  G4double mEnergyAfterProcess = 0.0;

  G4double mEnergyLossDuringProcess = 0.0;

  TVector3 mMomentumDirectionBeforeProcess = TVector3( 0, 0, 0);

  TVector3 mMomentumDirectionAfterProcess = TVector3( 0, 0, 0);

  TVector3 mProcessPosition = TVector3( 0, 0, 0);

  TVector3 mEmissionPointFromSource = TVector3( 0, 0, 0);

  TVector3 mEmissionMomentumDirectionFromSource = TVector3( 0, 0, 0);

  G4double mEmissionEnergyFromSource = 0.0;

  std::string mParticleName = "";

  G4int mParticlePGDCoding = -1;

  TVector3 mPolarizationBeforeProcess = TVector3( 0, 0, 0);

  TVector3 mPolarizationAfterProcess = TVector3( 0, 0, 0);

  std::string mProcessName = "";

  G4int mParentID = -1; 

  G4double mInteractionTime = 0.0;

  G4double mLocalTime = 0.0;

  G4double mGlobalTime = 0.0;

  G4double mProperTime = 0.0;

  //This value descibes how many times hit scattered in the same volume
  G4int mMultipleScatteringNumber = 0;

  // This value descibes which scattering for trackID is represented by hit
  G4int mScatteringIndex = 0;

  G4int mGammaSourceModel = 0;
  G4int mGammaKind = 0;
  TVector3 mInitialPolarization = TVector3( 0, 0, 0);

  bool mIsMerged = false;
};

#endif
