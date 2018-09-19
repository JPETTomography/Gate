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
 *  @file GateJPETActorMessenger.cc
 */

#include "../include/GateJPETActorMessenger.hh"
#ifdef G4ANALYSIS_USE_ROOT

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

#include "GateJPETActor.hh"

GateJPETActorMessenger::GateJPETActorMessenger(GateJPETActor* sensor) : GateActorMessenger(sensor),pActor(sensor)
{
	 BuildCommands(baseName+sensor->GetObjectName());
}

GateJPETActorMessenger::~GateJPETActorMessenger()
{
	delete pEnableXPositionCmd;
	delete pEnableYPositionCmd;
	delete pEnableZPositionCmd;
	delete pEnableParticleNameCmd;
	delete pEnableParticleCmd;
	delete pEnableTrackIDCmd;
	delete pEnableEventIDCmd;
	delete pEnableParentIDCmd;
	delete pEnableRunIDCmd;
	delete pEnableEkinCmd;
	delete pEnableInteractionTimeCmd;
	delete pEnableProcessNameCmd;
	delete pEnableEmissionPointCmd;
	delete pEnablePrimaryEnergyCmd;
	delete pEnableASCIFileNameCmd;
	delete pEnableProcessCmd;
}

void GateJPETActorMessenger::BuildCommands(G4String base)
{
	G4String guidance;
	G4String bb;

	bb = base+"/enableXPosition";
	pEnableXPositionCmd = new G4UIcmdWithABool(bb,this);
	guidance = "Save position of particles along X axis in the phase space file.";
	pEnableXPositionCmd->SetGuidance(guidance);
	pEnableXPositionCmd->SetParameterName("State",false);

	bb = base+"/enableYPosition";
	pEnableYPositionCmd = new G4UIcmdWithABool(bb,this);
	guidance = "Save position of particles along Y axis in the phase space file.";
	pEnableYPositionCmd->SetGuidance(guidance);
	pEnableYPositionCmd->SetParameterName("State",false);

	bb = base+"/enableZPosition";
	pEnableZPositionCmd = new G4UIcmdWithABool(bb,this);
	guidance = "Save position of particles along Z axis in the phase space file.";
	pEnableZPositionCmd->SetGuidance(guidance);
	pEnableZPositionCmd->SetParameterName("State",false);

	bb = base+"/enableParticleName";
	pEnableParticleNameCmd = new G4UIcmdWithABool(bb,this);
	guidance = "Save the name of particles in the phase space file.";
	pEnableParticleNameCmd->SetGuidance(guidance);
	pEnableParticleNameCmd->SetParameterName("State",false);

	bb = base+"/enableParticle";
	pEnableParticleCmd =  new G4UIcmdWithAString(bb,this);
	guidance = "Save data only for choosed particle.";
	pEnableParticleCmd->SetGuidance(guidance);
	pEnableParticleCmd->SetParameterName("Particle Name",false);

	bb = base+"/enableTrackID";
	pEnableTrackIDCmd =  new G4UIcmdWithABool(bb,this);
	guidance = "Save track ID";
	pEnableTrackIDCmd->SetGuidance(guidance);
	pEnableTrackIDCmd->SetParameterName("State",false);

	bb = base+"/enableEventID";
	pEnableEventIDCmd =  new G4UIcmdWithABool(bb,this);
	guidance = "Save event ID";
	pEnableEventIDCmd->SetGuidance(guidance);
	pEnableEventIDCmd->SetParameterName("State",false);

	bb = base+"/enableParentID";
	pEnableParentIDCmd =  new G4UIcmdWithABool(bb,this);
	guidance = "Save parent ID";
	pEnableParentIDCmd->SetGuidance(guidance);
	pEnableParentIDCmd->SetParameterName("State",false);

	bb = base+"/enableRunID";
	pEnableRunIDCmd =  new G4UIcmdWithABool(bb,this);
	guidance = "Save run ID";
	pEnableRunIDCmd->SetGuidance(guidance);
	pEnableRunIDCmd->SetParameterName("State",false);

	bb = base+"/enableEkin";
	pEnableEkinCmd =  new G4UIcmdWithABool(bb,this);
	guidance = "Save particle kinetic energy";
	pEnableEkinCmd->SetGuidance(guidance);
	pEnableEkinCmd->SetParameterName("State",false);

	bb = base+"/enableInteractionTime";
	pEnableInteractionTimeCmd = new G4UIcmdWithABool(bb,this);
	guidance = "Save particle interaction time";
	pEnableInteractionTimeCmd->SetGuidance(guidance);
	pEnableInteractionTimeCmd->SetParameterName("State",false);

	bb = base+"/enableProcessName";
	pEnableProcessNameCmd = new G4UIcmdWithABool(bb,this);
	guidance = "Save process name";
	pEnableProcessNameCmd->SetGuidance(guidance);
	pEnableProcessNameCmd->SetParameterName("State",false);

	bb = base+"/enableEmissionPoint";
	pEnableEmissionPointCmd = new G4UIcmdWithABool(bb,this);
	guidance = "Save particle's emission point.";
	pEnableEmissionPointCmd->SetGuidance(guidance);
	pEnableEmissionPointCmd->SetParameterName("State",false);

	bb = base+"/enablePrimaryEnergy";
	pEnablePrimaryEnergyCmd = new G4UIcmdWithABool(bb,this);
	guidance = "Save particle's primary energy";
	pEnablePrimaryEnergyCmd->SetGuidance(guidance);
	pEnablePrimaryEnergyCmd->SetParameterName("State",false);

	bb = base+"/enableASCIFile";
	pEnableASCIFileNameCmd =  new G4UIcmdWithAString(bb,this);
	guidance = "Save data about particle name and positions.";
	pEnableASCIFileNameCmd->SetGuidance(guidance);
	pEnableASCIFileNameCmd->SetParameterName("ASCI file name",false);

	bb = base+"/enableProcess";
	pEnableProcessCmd =  new G4UIcmdWithAString(bb,this);
	guidance = "Save data only for specific processes";
	pEnableProcessCmd->SetGuidance(guidance);
	pEnableProcessCmd->SetParameterName("Process Filter",false);
}

void GateJPETActorMessenger::SetNewValue(G4UIcommand* command, G4String param)
{
	if(command == pEnableXPositionCmd)
		pActor->SetXPositionEnabled(pEnableXPositionCmd->GetNewBoolValue(param));

	if(command == pEnableYPositionCmd)
		pActor->SetYPositionEnabled(pEnableYPositionCmd->GetNewBoolValue(param));

	if(command == pEnableZPositionCmd)
		pActor->SetZPositionEnabled(pEnableZPositionCmd->GetNewBoolValue(param));

	if(command == pEnableParticleNameCmd)
		pActor->SetParticleNameEnabled(pEnableParticleNameCmd->GetNewBoolValue(param));

	if(command ==pEnableParticleCmd)
		pActor->SetParticleEnabled(param);

	if(command == pEnableTrackIDCmd)
		pActor->SetTrackIDEnabled(pEnableTrackIDCmd->GetNewBoolValue(param));

	if(command == pEnableEventIDCmd)
		pActor->SetEventIDEnabled(pEnableEventIDCmd->GetNewBoolValue(param));

	if(command == pEnableParentIDCmd)
		pActor->SetParentIDEnabled(pEnableParentIDCmd->GetNewBoolValue(param));

	if(command == pEnableRunIDCmd)
		pActor->SetRunIDEnabled(pEnableRunIDCmd->GetNewBoolValue(param));

	if(command == pEnableEkinCmd)
		pActor->SetEkinEnabled(pEnableEkinCmd->GetNewBoolValue(param));

	if(command == pEnableInteractionTimeCmd)
		pActor->SetInteractionTimeEnabled(pEnableInteractionTimeCmd->GetNewBoolValue(param));

	if(command == pEnableProcessNameCmd)
		pActor->SetProcessNameEnabled(pEnableProcessNameCmd->GetNewBoolValue(param));

	if(command == pEnableEmissionPointCmd)
		pActor->SetEmissionPointEnabled(pEnableEmissionPointCmd->GetNewBoolValue(param));

	if(command == pEnablePrimaryEnergyCmd)
		pActor->SetPrimaryEnergy(pEnablePrimaryEnergyCmd->GetNewBoolValue(param));

	if(command == pEnableASCIFileNameCmd)
		pActor->SetASCIFileName(param);

	if(command == pEnableProcessCmd)
		pActor->SetProcessName(param);

	GateActorMessenger::SetNewValue(command ,param );
}
#endif
