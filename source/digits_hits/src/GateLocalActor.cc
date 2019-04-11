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
 *  @file GateLocalActor.cc
 */
#include "GateLocalActor.hh"
#include "GateGlobalActor.hh"
#include "GateGlobalActorMessenger.hh"

GateLocalActor::GateLocalActor(G4String name, G4int depth):GateVActor(name, depth)
{
	pMessenger = new GateGlobalActorMessenger(this);
}

GateLocalActor::~GateLocalActor()
{
	delete pMessenger;
}

void GateLocalActor::Construct()
{
	GateVActor::Construct();
	EnableUserSteppingAction(true);
	EnableBeginOfEventAction(true);
	EnableEndOfEventAction(true);
}

void GateLocalActor::UserSteppingAction(const GateVVolume *, const G4Step* step)
{
	GateGlobalActor::Instance()->NoticeStep(GetVolumeName(), step);
}

void GateLocalActor::BeginOfEventAction(const G4Event* event )
{
 GateGlobalActor::Instance()->NoticeBeginOfEvent(event->GetEventID());
}

void GateLocalActor::EndOfEventAction(const G4Event*)
{
 GateGlobalActor::Instance()->NoticeEndOfEvent();
}

void GateLocalActor::SaveData()
{
	GateGlobalActor::Instance()->Write();
}

void GateLocalActor::ResetData()
{
	GateGlobalActor::Instance()->Reset();
}
