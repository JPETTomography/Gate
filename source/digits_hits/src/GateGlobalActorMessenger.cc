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
 *  @file GateGlobalActorMessenger.cc
 */
#include "GateGlobalActorMessenger.hh"
#include "GateLocalActor.hh"
#include "GateGlobalActor.hh"
#include <cassert>

GateGlobalActorMessenger::GateGlobalActorMessenger(GateLocalActor* sensor) : GateActorMessenger(sensor), pActor(sensor)
{
	BuildCommands("");
}

GateGlobalActorMessenger::~GateGlobalActorMessenger()
{
	int commands_number = mCommands.size();
	for(int cmd_index = 0; cmd_index < commands_number; ++cmd_index)
		delete mCommands[cmd_index];
	mCommands.clear();
}

void GateGlobalActorMessenger::BuildCommands(G4String)
{
	BuildBoolCmds();
	BuildStringCmds();
	BuildIntegerCmds();
	BuildDoubleCmds();
	Build3VectorCmds();
}

void GateGlobalActorMessenger::SetNewValue(G4UIcommand* command, G4String parameter)
{
	if(SetNewBoolValue(command, parameter))
		return;
	else if(SetNewStringValue(command, parameter))
		return;
	else if(SetNewIntegerValue(command, parameter))
		return;
	else if(SetNewDoubleValue(command, parameter))
		return;
	else if(SetNew3VectorValue(command, parameter))
		return;
	GateActorMessenger::SetNewValue(command ,parameter);
}

void GateGlobalActorMessenger::AddSimpleCmd(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWithoutParameter** cmd_pointer)
{
	assert(cmd_name.size() > 0 );
	assert(cmd_guidance.size() > 0 );
	*cmd_pointer = new G4UIcmdWithoutParameter(GetCmdPath(cmd_name),this);
	(*cmd_pointer)->SetGuidance(cmd_guidance);
	mCommands.push_back(*cmd_pointer);
}

void GateGlobalActorMessenger::AddBoolCmd(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWithABool** cmd_pointer)
{
	assert(cmd_name.size() > 0 );
	assert(cmd_guidance.size() > 0 );
	*cmd_pointer = new G4UIcmdWithABool(GetCmdPath(cmd_name),this);
	(*cmd_pointer)->SetGuidance(cmd_guidance);
	(*cmd_pointer)->SetParameterName(cmd_name,false);
	mCommands.push_back(*cmd_pointer);
}

void GateGlobalActorMessenger::AddStringCmd(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWithAString** cmd_pointer)
{
	assert(cmd_name.size() > 0 );
	assert(cmd_guidance.size() > 0 );
	*cmd_pointer = new G4UIcmdWithAString(GetCmdPath(cmd_name),this);
	(*cmd_pointer)->SetGuidance(cmd_guidance);
	(*cmd_pointer)->SetParameterName(cmd_name,false);
	mCommands.push_back(*cmd_pointer);
}

void GateGlobalActorMessenger::AddIntegerCmd(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWithAnInteger** cmd_pointer)
{
	assert(cmd_name.size() > 0 );
	assert(cmd_guidance.size() > 0 );
	*cmd_pointer = new G4UIcmdWithAnInteger(GetCmdPath(cmd_name),this);
	(*cmd_pointer)->SetGuidance(cmd_guidance);
	(*cmd_pointer)->SetParameterName(cmd_name,false);
	mCommands.push_back(*cmd_pointer);
}

void GateGlobalActorMessenger::AddDoubleCmdWithoutUnit(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWithADouble** cmd_pointer)
{
	assert(cmd_name.size() > 0 );
	assert(cmd_guidance.size() > 0 );
	*cmd_pointer = new G4UIcmdWithADouble(GetCmdPath(cmd_name),this);
	(*cmd_pointer)->SetGuidance(cmd_guidance);
	(*cmd_pointer)->SetParameterName(cmd_name,false);
	mCommands.push_back(*cmd_pointer);
}

void GateGlobalActorMessenger::AddDoubleCmdWithUnit(const G4String& cmd_name, const G4String& cmd_guidance, const G4String& default_unit, const G4String& unit_candidates, const G4String& unit_category, G4UIcmdWithADoubleAndUnit** cmd_pointer)
{
	assert(cmd_name.size() > 0 );
	assert(cmd_guidance.size() > 0 );
	assert(default_unit.size() > 0 || unit_candidates.size() > 0 || unit_category.size() > 0 );
	*cmd_pointer = new G4UIcmdWithADoubleAndUnit(GetCmdPath(cmd_name),this);
	(*cmd_pointer)->SetGuidance(cmd_guidance);
	(*cmd_pointer)->SetParameterName(cmd_name,false);
	mCommands.push_back(*cmd_pointer);

	if(default_unit.size() > 0 )
		(*cmd_pointer)->SetDefaultUnit(default_unit.c_str());
	if(unit_candidates.size() > 0 )
		(*cmd_pointer)->SetUnitCandidates(unit_candidates.c_str());
	if(unit_category.size() > 0 )
		(*cmd_pointer)->SetUnitCategory(unit_category.c_str());
}

void GateGlobalActorMessenger::Add3VectorCmdWithoutUnit(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWith3Vector** cmd_pointer)
{
	assert(cmd_name.size() > 0 );
	assert(cmd_guidance.size() > 0 );
	*cmd_pointer = new G4UIcmdWith3Vector(GetCmdPath(cmd_name),this);
	(*cmd_pointer)->SetGuidance(cmd_guidance);
	(*cmd_pointer)->SetParameterName(std::string(cmd_name + "_x").c_str(),std::string(cmd_name + "y").c_str(),std::string(cmd_name + "z").c_str(),false);
	mCommands.push_back(*cmd_pointer);
}

void GateGlobalActorMessenger::Add3VectorCmdWithUnit(const G4String& cmd_name, const G4String& cmd_guidance, const G4String& default_unit, const G4String& unit_candidates, const G4String& unit_category, G4UIcmdWith3VectorAndUnit** cmd_pointer)
{
	assert(cmd_name.size() > 0 );
	assert(cmd_guidance.size() > 0 );
	*cmd_pointer = new G4UIcmdWith3VectorAndUnit(GetCmdPath(cmd_name),this);
	(*cmd_pointer)->SetGuidance(cmd_guidance);
	(*cmd_pointer)->SetParameterName(std::string(cmd_name + "_x").c_str(),std::string(cmd_name + "y").c_str(),std::string(cmd_name + "z").c_str(),false);
	mCommands.push_back(*cmd_pointer);

	if(default_unit.size() > 0 )
		(*cmd_pointer)->SetDefaultUnit(default_unit.c_str());
	if(unit_candidates.size() > 0 )
		(*cmd_pointer)->SetUnitCandidates(unit_candidates.c_str());
	if(unit_category.size() > 0 )
		(*cmd_pointer)->SetUnitCategory(unit_category.c_str());
}

G4bool GateGlobalActorMessenger::GetCommandValue(G4UIcmdWithABool*, const G4String& parameter) const
{
	assert( parameter.size() > 0 );
	return G4UIcmdWithABool::GetNewBoolValue(parameter);
}

G4String GateGlobalActorMessenger::GetCommandValue(G4UIcmdWithAString*, const G4String& parameter) const
{
	assert( parameter.size() > 0);
	return parameter;
}

G4int GateGlobalActorMessenger::GetCommandValue(G4UIcmdWithAnInteger*, const G4String& parameter) const
{
	assert( parameter.size() > 0 );
	return G4UIcmdWithAnInteger::GetNewIntValue(parameter);
}

G4double GateGlobalActorMessenger::GetCommandValue(G4UIcmdWithADouble*, const G4String& parameter) const
{
	assert( parameter.size() > 0 );
	return G4UIcmdWithADouble::GetNewDoubleValue(parameter);
}

G4double GateGlobalActorMessenger::GetCommandValue(G4UIcmdWithADoubleAndUnit*, const G4String& parameter) const
{
	assert( parameter.size() > 0 );
	return G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(parameter);
}

G4ThreeVector GateGlobalActorMessenger::GetCommandValue(G4UIcmdWith3Vector*, const G4String& parameter) const
{
	assert( parameter.size() > 0 );
	return G4UIcmdWith3Vector::GetNew3VectorValue(parameter);
}

G4ThreeVector GateGlobalActorMessenger::GetCommandValue(G4UIcmdWith3VectorAndUnit*, const G4String& parameter) const
{
	assert( parameter.size() > 0 );
	return G4UIcmdWith3VectorAndUnit::GetNew3VectorValue(parameter);
}

G4String GateGlobalActorMessenger::GetCmdPath(const G4String& cmd_name)
{
	return baseName + pActor->GetObjectName() + "/global/" + cmd_name;
}

/******************************************************************Code below if for adding and handling commands**********************************************************************************************/

void GateGlobalActorMessenger::BuildBoolCmds()
{
	//Here add your G4bool type command (use command AddBoolCmd to add new one).
	AddSimpleCmd("enableVolumeName", "Save volume (layer) name ", &pEnableVolumeNameCmd);
	AddSimpleCmd("enableScintilatorPosition", "Save scintilator position (easy way to distinguish scintilators in the same volume (layer)", &pEnableScintilatorPositionCmd);
	AddSimpleCmd("enableEventID", "Save event ID", &pEnableEventIDCmd);
	AddSimpleCmd("enableTrackID", "Save track ID", &pEnableTrackIDCmd);
	AddSimpleCmd("enableEnergyBeforeProcess", "Save particle energy before process", &pEnableEnergyBeforeProcessCmd);
	AddSimpleCmd("enableEnergyAfterProcess", "Save particle energy after process", &pEnableEnergyAfterProcessCmd);
	AddSimpleCmd("enableEnergyLossDuringProcess", "Save particle energy different beteen before and after process", &pEnableEnergyLossDuringProcessCmd);
	AddSimpleCmd("enableMomentumDirectionBeforeProcess", "Save particle momentum direction before process", &pEnableMomentumDirectionBeforeProcessCmd);
	AddSimpleCmd("enableMomentumDirectionAfterProcess", "Save particle momentum direction after process", &pEnableMomentumDirectionAfterProcessCmd);
	AddSimpleCmd("enableProcessPosition", "Save process name", &pEnableProcessPositionCmd);
	AddSimpleCmd("enableEmissionPointFromSource","Save information about emission point from source",&pEnableEmissionPointFromSourceCmd);
	AddSimpleCmd("enableEmissionMomentumDirectionFromSource", "Save particle momentum direction from source", &pEnableEmissionMomentumDirectionFromSourceCmd);
	AddSimpleCmd("enableEmissionEnergyFromSource", "Save particle energy from source", &pEnableEmissionEnergyFromSourceCmd);
	AddSimpleCmd("enableParticleName", "Save particle name", &pEnableParticleNameCmd);
	AddSimpleCmd("enableParticlePGDCoding", "Save particle PDG code", &pEnableParticlePGDCodingCmd);
	AddSimpleCmd("enableProcessAngle", "Save angle between particle momentum direction before and after process", &pEnableProcessAngleCmd);
	AddSimpleCmd("enablePolarizationBeforeProcess", "Save particle polarization before process", &pEnablePolarizationBeforeProcessCmd);
	AddSimpleCmd("enablePolarizationAfterProcess", "Save particle polarization after process", &pEnablePolarizationAfterProcessCmd);
	AddSimpleCmd("enableProcessName", "Save process name", &pEnableProcessNameCmd);
	AddSimpleCmd("enableParentID", "Save parent ID", &pEnableParentIDCmd);
	AddSimpleCmd("enableInteractionTime", "Save interaction time (time of step)", &pEnableInteractionTimeCmd);
	AddSimpleCmd("enableLocalTime", "Save local time (time since the track was created)", &pEnableLocalTimeCmd);
	AddSimpleCmd("enableGlobalTime", "Save global time (time since the event was created)", &pEnableGlobalTimeCmd);
	AddSimpleCmd("enableProperTime", "Save proper time (time in its rest frame since the track was created)", &pEnableProperTimeCmd);
	AddSimpleCmd("enableUseAdder", "Use Global Actor Adder", &pEnableUseAdderCmd );
	
}

void GateGlobalActorMessenger::BuildStringCmds()
{
	//Here add your G4String type command (use command AddStringCmd to add new one).
	AddStringCmd("saveTo", "ROOT file name where data from GlobalActor will be saved", &pFileNameCmd);
	AddStringCmd("filterProcessName", "Save data only for process with this name. You can call this command more then once to add more processes names.", &pFilterProcessNameCmd);
	AddStringCmd("filterParticleName", "Save data only for particle with this name. You can call this command more then once to add more particle names.", &pFilterParticleNameCmd);
	AddStringCmd("filterIgnoreProcessName", "Save data only for process without this name. You can call this command more then once to add more processes names.", &pFilterIgnoreProcessNameCmd);
}
void GateGlobalActorMessenger::BuildIntegerCmds()
{
	//Here add your G4inttype command (use command AddIntegerCmd to add new one).
	AddIntegerCmd("filterParticlePDGCode", "Save data only for particle with this PDG code. You can call this command more then once to add more processes names.", &pFilterParticlePDGCodeCmd);
}

void GateGlobalActorMessenger::BuildDoubleCmds()
{
	//Here add your G4double type command (use command AddDoubleCmdWithUnit or AddDoubleCmdWithoutUnit to add new one).
	AddDoubleCmdWithUnit("filterProcessAngle", "Save data only for specific compton angle", "deg", "", "", &pFilterProcessAngleCmd);
	AddDoubleCmdWithoutUnit("filterAdderTimeInterval", "Time interval (ns) between two hits in the same scintillator when we threat them as one hit", &pFilterAdderTimeInterval );
}

void GateGlobalActorMessenger::Build3VectorCmds()
{
	//Here add your G4ThreeVector type command (use command Add3VectorCmdWithoutUnit or Add3VectorCmdWithUnit to add new one).
	Add3VectorCmdWithUnit("filterEmissionPoint","Save data only form particles emitted from this point.","","m cm mm","", &pFilterEmissionPointCmd);
}

G4bool GateGlobalActorMessenger::SetNewBoolValue(G4UIcommand* command, G4String /*parameter*/)
{
	//Here add your G4bool type command handling.
	if(command == pEnableVolumeNameCmd)
			GateGlobalActor::Instance()->SetEnableVolumeName();
	else if(command == pEnableScintilatorPositionCmd)
			GateGlobalActor::Instance()->SetEnableScintilatorPosition();
	else if(command == pEnableEventIDCmd)
			GateGlobalActor::Instance()->SetEnableEventID();
	else if(command == pEnableTrackIDCmd)
			GateGlobalActor::Instance()->SetEnableTrackID();
	else if(command == pEnableEnergyBeforeProcessCmd)
			GateGlobalActor::Instance()->SetEnableEnergyBeforeProcess();
	else if(command == pEnableEnergyAfterProcessCmd)
			GateGlobalActor::Instance()->SetEnableEnergyAfterProcess();
	else if(command == pEnableEnergyLossDuringProcessCmd)
			GateGlobalActor::Instance()->SetEnableEnergyLossDuringProcess();
	else if(command == pEnableMomentumDirectionBeforeProcessCmd)
			GateGlobalActor::Instance()->SetEnableMomentumDirectionBeforeProcess();
	else if(command == pEnableMomentumDirectionAfterProcessCmd)
			GateGlobalActor::Instance()->SetEnableMomentumDirectionAfterProcess();
	else if(command == pEnableProcessPositionCmd)
			GateGlobalActor::Instance()->SetEnableProcessPosition();
	else if(command == pEnableEmissionPointFromSourceCmd)
			GateGlobalActor::Instance()->SetEnableEmissionPointFromSource();
	else if(command == pEnableEmissionMomentumDirectionFromSourceCmd)
			GateGlobalActor::Instance()->SetEnableEmissionMomentumDirectionFromSource();
	else if(command == pEnableEmissionEnergyFromSourceCmd)
			GateGlobalActor::Instance()->SetEnableEmissionEnergyFromSource();
	else if(command == pEnableParticleNameCmd)
			GateGlobalActor::Instance()->SetEnableParticleName();
	else if(command == pEnableParticlePGDCodingCmd)
			GateGlobalActor::Instance()->SetEnableParticlePGDCoding();
	else if(command == pEnableProcessAngleCmd)
			GateGlobalActor::Instance()->SetEnableProcessAngle();
	else if(command == pEnablePolarizationBeforeProcessCmd)
			GateGlobalActor::Instance()->SetEnablePolarizationBeforeProcess();
	else if(command == pEnablePolarizationAfterProcessCmd)
			GateGlobalActor::Instance()->SetEnablePolarizationAfterProcess();
	else if(command == pEnableProcessNameCmd)
			GateGlobalActor::Instance()->SetEnableProcessName();
	else if(command == pEnableParentIDCmd)
			GateGlobalActor::Instance()->SetEnableParentID();
	else if(command == pEnableInteractionTimeCmd)
			GateGlobalActor::Instance()->SetEnableInteractionTime();
	else if(command == pEnableLocalTimeCmd)
			GateGlobalActor::Instance()->SetEnableLocalTime();
	else if(command == pEnableGlobalTimeCmd)
			GateGlobalActor::Instance()->SetEnableGlobalTime();
	else if(command == pEnableProperTimeCmd)
			GateGlobalActor::Instance()->SetEnableProperTime();
	else if(command == pEnableUseAdderCmd)
			GateGlobalActor::Instance()->SetEnableAdder();
	else
		return false;
	return true;
}

G4bool GateGlobalActorMessenger::SetNewStringValue(G4UIcommand* command, G4String parameter)
{
	//Here add your G4String type command handling.
	if(command == pFileNameCmd)
		GateGlobalActor::Instance()->SetFileName(GetCommandValue(pFileNameCmd, parameter));
	else if(command == pFilterProcessNameCmd)
		GateGlobalActor::Instance()->SetFilterProcessName(GetCommandValue(pFilterProcessNameCmd, parameter));
	else if(command == pFilterParticleNameCmd)
		GateGlobalActor::Instance()->SetFilterParticleName(GetCommandValue(pFilterParticleNameCmd, parameter));
	else if(command == pFilterIgnoreProcessNameCmd)
		GateGlobalActor::Instance()->SetFilterIgnoreProcessName(GetCommandValue(pFilterIgnoreProcessNameCmd, parameter));
	else
		return false;
	return true;
}

G4bool GateGlobalActorMessenger::SetNewIntegerValue(G4UIcommand* command, G4String parameter)
{
	//Here add your G4int type command handling.
	if(command == pFilterParticlePDGCodeCmd)
		GateGlobalActor::Instance()->SetFilerParticlePDGCode(GetCommandValue(pFilterParticlePDGCodeCmd, parameter));
	else
		return false;
	return true;
}

G4bool GateGlobalActorMessenger::SetNewDoubleValue(G4UIcommand* command, G4String parameter)
{
	//Here add your G4double type command handling.
	if(command == pFilterProcessAngleCmd)
		GateGlobalActor::Instance()->SetFilerProcessAngle(GetCommandValue(pFilterProcessAngleCmd, parameter));
	else if(command == pFilterAdderTimeInterval )
		GateGlobalActor::Instance()->SetTimeIntervalBetweenHits(GetCommandValue(pFilterAdderTimeInterval, parameter));
	else
		return false;
	return true;
}

G4bool GateGlobalActorMessenger::SetNew3VectorValue(G4UIcommand* command, G4String parameter)
{
	//Here add your G4ThreeVector type command handling.
	if(command == pFilterEmissionPointCmd)
		GateGlobalActor::Instance()->SetFilterEmissionPoint(GetCommandValue(pFilterEmissionPointCmd, parameter));
	else
		return false;
	return true;
}
