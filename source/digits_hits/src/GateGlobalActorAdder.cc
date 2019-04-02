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
 *  @file GateGlobalActorAdder.cc
 */

#include "GateGlobalActorAdder.hh"
#include "TMath.h"
#include <iostream>

GateGlobalActorAdder::GateGlobalActorAdder() {}

GateGlobalActorAdder::~GateGlobalActorAdder() {}

/**
This method takes a hit and if this hit represent signal from particle emitted from source ( parentID=0) a classification begins.
Rules for classification:
1.  first hit in the scintillator - add it to hits list
2.  next hit in the scintillator but time interval betwean them is greater than mTimeInterval - add it to hits hits list
3. otherwise : merge hit with last hits which happened in this scintillator ( merge here means update of energy deposition, post momemntum, polarization and final energy ).
**/
void GateGlobalActorAdder::processHit( const GateGlobalActorHit& hit )
{
 /**
 We only accept energy deposition from particles emitted from source - any other always have parentID!=0
 **/
 if ( hit.getParentID() != 0 ) { return; }

 mCurrentProcessedEvent = hit.getEventID();
 GateGlobalActorHits::reverse_iterator found = findLastHitInTheSameScintillator( hit );
 if ( found == mHits.rend() || !isCommonTimeInterval( *found, hit ) ) { insertHit( hit ); }
 else { mergeHits( hit, *found ); }
}

GateGlobalActorHits GateGlobalActorAdder::getHits() const { return mHits; }
  
void GateGlobalActorAdder::setTimeIntervalBetweenHits( const G4double& time ) { mTimeInterval = time; }

void GateGlobalActorAdder::reset() { mHits.clear(); mCurrentProcessedEvent = cNullEvent; }

bool GateGlobalActorAdder::isReadyToMerge( const GateGlobalActorHit& hit ) const
{
 if ( mCurrentProcessedEvent == cNullEvent ) { return false; }
 if ( mCurrentProcessedEvent == hit.getEventID() ) { return false; }
 return true;
}

GateGlobalActorHits::reverse_iterator GateGlobalActorAdder::findLastHitInTheSameScintillator( const GateGlobalActorHit& hit )
{
 GateGlobalActorHits::reverse_iterator found_hit =  mHits.rend();

 for ( GateGlobalActorHits::reverse_iterator ihit = mHits.rbegin(); ihit != mHits.rend(); ++ihit )
 {
  if ( ihit->isTheSameScintillator( hit ) )
  {
   found_hit = ihit;
   break;
  }
 }
 return found_hit;
}

bool GateGlobalActorAdder::isCommonTimeInterval( const GateGlobalActorHit& hit_a, const GateGlobalActorHit& hit_b ) const { return TMath::Abs( hit_a.getLocalTime() - hit_b.getLocalTime() ) <= mTimeInterval; }

void GateGlobalActorAdder::insertHit( const GateGlobalActorHit& hit ) { mHits.push_back( hit ); }

void GateGlobalActorAdder::mergeHits( const GateGlobalActorHit& hit_source, GateGlobalActorHit& hit_to_update )
{
 G4double energy_loss = hit_to_update.getEnergyLossDuringProcess() + hit_source.getEnergyLossDuringProcess();
 hit_to_update.setEnergyLossDuringProcess( energy_loss );
 
 G4double energy_after = hit_to_update.getEnergyAfterProcess() - hit_source.getEnergyLossDuringProcess();
 hit_to_update.setEnergyAfterProcess( energy_after );
 
 hit_to_update.setMomentumDirectionAfterProcess( hit_source.getMomentumDirectionAfterProcess() );
 hit_to_update.setPolarizationAfterProcess( hit_source.getPolarizationAfterProcess() );

 hit_to_update.setInteractionTime( hit_to_update.getInteractionTime() + hit_source.getInteractionTime() );
}

G4int GateGlobalActorAdder::getCurrentProcessedEventID() const { return mCurrentProcessedEvent; }
