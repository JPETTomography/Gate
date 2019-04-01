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
 *  @file GateGlobalActorAdder.hh
 */

#ifndef GateGlobalActorAdder_hh
#define GateGlobalActorAdder_hh

#include "GateGlobalActorHit.hh"
#include <vector>

typedef std::vector< GateGlobalActorHit > GateGlobalActorHits;

/**
 * @About: This class represents digitalization of hits seen by scintillator based detector. If photo scattered more than once in the same scintillato and this scatterings are in time interval
 * chosen by user they are merged in one hit. Final hit has pre variables (before the Compton scattering) from first hit and post values from last hit. Final energy deposition is equal sum of all
 * energy depositions from hits.
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
**/
class GateGlobalActorAdder
{
 public:
  GateGlobalActorAdder();
  ~GateGlobalActorAdder();

  void processHit( const GateGlobalActorHit& hit );

  GateGlobalActorHits getHits() const;
  
  void setTimeIntervalBetweenHits( const G4double& time );

  void reset();

  bool isReadyToMerge( const GateGlobalActorHit& hit ) const;

  G4int getCurrentProcessedEventID() const;

 private:
  GateGlobalActorHits::reverse_iterator findLastHitInTheSameScintillator( const GateGlobalActorHit& hit );
  bool isCommonTimeInterval( const GateGlobalActorHit& hit_a, const GateGlobalActorHit& hit_b ) const;
  void insertHit( const GateGlobalActorHit& hit );
  void mergeHits( const GateGlobalActorHit& hit_source, GateGlobalActorHit& hit_to_update );

 private:
  GateGlobalActorHits mHits;
  G4double mTimeInterval = 0.0;//ns
  const G4int cNullEvent = -1;
  G4int mCurrentProcessedEvent = cNullEvent;
   
};

#endif
