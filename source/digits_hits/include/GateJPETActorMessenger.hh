/**
 *  @copyright Copyright 2017 The J-PET Gate Authors. All rights reserved.
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
 *  @file GateJPETActorMessenger.hh
 */

#include "GateConfiguration.h"
#ifdef G4ANALYSIS_USE_ROOT
#ifndef GATEJPETACTORMESSENGER_HH
#define GATEJPETACTORMESSENGER_HH
#include "globals.hh"
#include "GateActorMessenger.hh"

class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class GateJPETActor;

/**
 * Actor messenger created for JPET use
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateJPETActorMessenger : public GateActorMessenger
{
public:
	GateJPETActorMessenger(GateJPETActor* sensor);
	virtual ~GateJPETActorMessenger();

	void BuildCommands(G4String base);
	virtual void SetNewValue(G4UIcommand*, G4String);
protected:
	GateJPETActor* pActor;

	G4UIcmdWithABool* pEnableXPositionCmd;
	G4UIcmdWithABool* pEnableYPositionCmd;
	G4UIcmdWithABool* pEnableZPositionCmd;
	G4UIcmdWithABool* pEnableParticleNameCmd;
	G4UIcmdWithAString* pEnableParticleCmd;
	G4UIcmdWithABool* pEnableTrackIDCmd;
	G4UIcmdWithABool* pEnableEventIDCmd;
	G4UIcmdWithABool* pEnableParentIDCmd;
	G4UIcmdWithABool* pEnableRunIDCmd;
	G4UIcmdWithABool* pEnableEkinCmd;
	G4UIcmdWithABool* pEnableInteractionTimeCmd;
	G4UIcmdWithABool* pEnableProcessNameCmd;
	G4UIcmdWithABool* pEnableEmissionPointCmd;
	G4UIcmdWithABool* pEnablePrimaryEnergyCmd;
	G4UIcmdWithAString* pEnableASCIFileNameCmd;
};
#endif //GATEJPETACTORMESSANGER_HH/
#endif //G4ANALYSIS_USE_ROOT
