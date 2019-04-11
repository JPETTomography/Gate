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
 *  @file GateLocalActor.hh
 */

#ifndef GATELOCALACTOR_HH
#define GATELOCALACTOR_HH
#include "GateConfiguration.h"
#include "GateVActor.hh"
#include "GateGlobalActorMessenger.hh"

/**
 * @About class: This actor is element of system which provide to collect data from whole the simulation system.
 * There are two way of use this actor:
 * 1. like a node for global data collecting by GateGlobalActor,
 * 2. like a method to extend classical actor properties to support global data collecting.
 * This actor only communicates with GateGlobalActor and GateGlobalActorMessenger - no more no less.
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateLocalActor : public GateVActor
{
public:
	virtual ~GateLocalActor();
	// This macro initialize the CreatePrototype and CreateInstance
	FCT_FOR_AUTO_CREATOR_ACTOR(GateLocalActor)
	// Constructs the sensor
	virtual void Construct() override;

	virtual void UserSteppingAction(const GateVVolume *, const G4Step*) override;

	virtual void BeginOfEventAction(const G4Event* event ) override;

	virtual void EndOfEventAction(const G4Event*) override;

	/** Saves the data collections to the file
	 */
	virtual void SaveData() override;
	/** Overwrite function from GateVActor.
	 * This function used by GateVActor::BeginOfRunAction. If you want to reset data in file for each new run, call EnableResetDataAtEachRun(true).
	 * By default this function is not used;
	 */
	virtual void ResetData() override;
protected:
	GateLocalActor(G4String name, G4int depth=0);
private:
	GateGlobalActorMessenger* pMessenger;
};
MAKE_AUTO_CREATOR_ACTOR(LocalActor,GateLocalActor)

#endif
