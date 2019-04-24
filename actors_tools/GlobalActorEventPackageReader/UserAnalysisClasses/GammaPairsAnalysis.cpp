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
 *  @file GammaPairsAnalysis.cpp
 */
#include<iostream>
#include "GammaPairsAnalysis.h"
#include "../AnalysisClassesManager/AnalysisClassesManager.h"
#include <vector>
#include <cassert>

GammaPairsAnalysis::GammaPairsAnalysis() { }

GammaPairsAnalysis::GammaPairsAnalysis( std::string class_name ) { AnalysisClassesManager::getInstance()->registerClass( class_name, new GammaPairsAnalysis() ); }

GammaPairsAnalysis::~GammaPairsAnalysis() { }

void GammaPairsAnalysis::countThis( unsigned int track_1_hits_number, unsigned int track_2_hits_number, bool is_true )
{
 unsigned int i1 = 0, i2 = 0;
 if ( track_1_hits_number <= track_2_hits_number ) { i1 = track_1_hits_number; i2 = track_2_hits_number; }
 else { i1 = track_2_hits_number; i2 = track_1_hits_number; }

 std::string kind = "g" + std::to_string( i1 ) + ":g" + std::to_string( i2 );
 std::map<std::string, ScatterindKind>::iterator found = mKinds.find( kind );
 if ( found == mKinds.end() ) { mKinds.emplace( kind, ScatterindKind() ); }
 mKinds[kind].count( is_true );
}

std::string GammaPairsAnalysis::ScatterindKind::getStatus() const
{
 double false_fraction = 100.0 * static_cast<double>( nFalse ) / static_cast<double>( nTotal );
 std::string status = std::to_string( nTotal ) + " [ FALSE = " + std::to_string( false_fraction ) + " % ]";
 return status;
}

void GammaPairsAnalysis::ScatterindKind::count( bool is_true )
{
 ++nTotal;
 if ( !is_true ) { ++nFalse; }
}

void GammaPairsAnalysis::displayStatus() const
{
 std::cout << "eventsNumber = " << mEventsNumber << std::endl;
 for ( std::map<std::string, ScatterindKind>::const_iterator it = mKinds.cbegin(); it != mKinds.cend(); ++it ) 
 { std::cout << it->first << " =  " << it->second.getStatus() << std::endl; }
}

bool GammaPairsAnalysis::isNotMergedTrack( const Dictionaries::Track& track ) const
{
 for ( std::vector<Dictionaries::Hit>::const_iterator ihit = track.fHits.cbegin(); ihit != track.fHits.cend(); ++ihit )
 {
  if ( ihit->fHitKind == Dictionaries::Enums::HitKind::HitMerged ) { return false; }
 }
 return true;
}

void GammaPairsAnalysis::noticeEvent( const Dictionaries::Event& event )
{
 ++mEventsNumber;
 unsigned int tracks_number = event.getTracksNumber();

 assert( event.fTracks[0].fParentID == 0 );
 unsigned int track_1_hits_number = event.fTracks[0].getHitsNumber();
 bool is_true = isNotMergedTrack( event.fTracks[0] );

 unsigned int track_2_hits_number = 0;
 
 if ( tracks_number > 1 ) 
 { 
  assert( event.fTracks[1].fParentID == 0 );
  track_2_hits_number = event.fTracks[1].getHitsNumber(); 
  is_true = is_true && isNotMergedTrack( event.fTracks[1] ); 
 }

 countThis( track_1_hits_number, track_2_hits_number, is_true );
}
