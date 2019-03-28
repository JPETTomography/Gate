#ifndef GateGlobalActorHit_h
#define GateGlobalActorHit_h

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"
#include <string>
#include "TVector3.h"

class GateGlobalActorHit
{
 public:
  GateGlobalActorHit();
  GateGlobalActorHit( const G4Step& step, const std::string& volume_name );
  GateGlobalActorHit( const GateGlobalActorHit& hit );
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

  // comparing methods
  bool isTheSameScintillator( const GateGlobalActorHit& hit ) const;
  bool isTheSameEventID( const GateGlobalActorHit& hit ) const;
  bool isTheSameTrackID( const GateGlobalActorHit& hit ) const;

 private:
  void extractDataFromStep( const G4Step& step );
  TVector3 getTVector3( const G4ThreeVector& input_vector ) const;
  G4double keV( const G4double& energy ) const;

 private:
  std::string mVolumeName;

  TVector3 mScintillatorPosition;

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

  TVector3 mPolarizationBeforeProcess;

  TVector3 mPolarizationAfterProcess;

  std::string mProcessName;

  G4int mParentID;

  G4double mInteractionTime;

  G4double mLocalTime;

  G4double mGlobalTime;

  G4double mProperTime;
};

#endif
