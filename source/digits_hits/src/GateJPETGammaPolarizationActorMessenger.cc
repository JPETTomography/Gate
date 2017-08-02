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
 *  @file GateJPETGammaPolarizationActorMessenger.cc
 */

#include "GateJPETGammaPolarizationActorMessenger.hh"
#ifdef G4ANALYSIS_USE_ROOT
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "GateJPETGammaPolarizationActor.hh"

GateJPETGammaPolarizationActorMessenger::GateJPETGammaPolarizationActorMessenger(GateJPETGammaPolarizationActor* sensor) :  GateActorMessenger(sensor),pActor(sensor)
{
	BuildCommands(baseName+sensor->GetObjectName());
}

GateJPETGammaPolarizationActorMessenger::~GateJPETGammaPolarizationActorMessenger()
{
	delete pEnableLogFile;
	delete pEnableDiagnosticFile;
	delete pEnableUsePhiFlag;
	delete pEnableUseThetaFlag;
	delete pEnablePhiLimes;
	delete pEnableThetaLimes;
	delete pEnablePhiEpsilon;
	delete pEnableThetaEpsilon;
	delete pEnableCalcAllPhiAsNoNegative;
}

void GateJPETGammaPolarizationActorMessenger::BuildCommands(G4String base)
{
	G4String guidance;
	G4String bb;
	bb = base+"/enableLogFile";
	pEnableLogFile = new G4UIcmdWithAString(bb,this);
	guidance = "Log file";
	pEnableLogFile->SetGuidance(guidance);
	pEnableLogFile->SetParameterName("LogFile",false);

	bb = base+"/enableDiagnosticFile";
	pEnableDiagnosticFile = new G4UIcmdWithAString(bb,this);
	guidance = "Diagnostic file";
	pEnableDiagnosticFile->SetGuidance(guidance);
	pEnableDiagnosticFile->SetParameterName("GiagnosticFile",false);


	bb = base+"/enablePhiFlag";
	pEnableUsePhiFlag = new G4UIcmdWithABool(bb,this);

	bb = base+"/enableThetaFlag";
	pEnableUseThetaFlag = new G4UIcmdWithABool(bb,this);

	bb = base+"/enablePhiLimes";
	pEnablePhiLimes = new G4UIcmdWithADoubleAndUnit(bb,this);

	bb = base+"/enableThetaLimes";
	pEnableThetaLimes = new G4UIcmdWithADoubleAndUnit(bb,this);

	bb = base+"/enablePhiEpsilon";
	pEnablePhiEpsilon = new G4UIcmdWithADoubleAndUnit(bb,this);

	bb = base+"/enableThetaEpsilon";
	pEnableThetaEpsilon = new G4UIcmdWithADoubleAndUnit(bb,this);

	bb = base+"/enableAllPhiAsNoNegative";
	pEnableCalcAllPhiAsNoNegative = new G4UIcmdWithABool(bb,this);
}

void GateJPETGammaPolarizationActorMessenger::SetNewValue(G4UIcommand* command, G4String param)
{
	if(command == pEnableLogFile)
		pActor->SetLogFile(param);
	if(command == pEnableDiagnosticFile)
		pActor->SetDiagnosticFile(param);

	if(command == pEnableUsePhiFlag)
		pActor->SetPhiFlagParameter(pEnableUsePhiFlag->GetNewBoolValue(param), GateJPETGammaPolarizationActor::PhiThetaValueMode::Flag);

	if(command ==  pEnableUseThetaFlag)
		pActor->SetThetaFlagParameter(pEnableUseThetaFlag->GetNewBoolValue(param), GateJPETGammaPolarizationActor::PhiThetaValueMode::Flag);

	if(command == pEnablePhiLimes)
		pActor->SetPhiFlagParameter(pEnablePhiLimes->GetNewDoubleValue(param), GateJPETGammaPolarizationActor::PhiThetaValueMode::Limes);

	if(command == pEnableThetaLimes)
		pActor->SetThetaFlagParameter(pEnableThetaLimes->GetNewDoubleValue(param), GateJPETGammaPolarizationActor::PhiThetaValueMode::Limes);

	if(command == pEnablePhiEpsilon)
		pActor->SetPhiFlagParameter(pEnablePhiEpsilon->GetNewDoubleValue(param), GateJPETGammaPolarizationActor::PhiThetaValueMode::Epsilon);

	if(command == pEnableThetaEpsilon)
		pActor->SetThetaFlagParameter(pEnableThetaEpsilon->GetNewDoubleValue(param), GateJPETGammaPolarizationActor::PhiThetaValueMode::Epsilon);

	if(command == pEnableCalcAllPhiAsNoNegative)
		pActor->SetAllPhiAsNoNegative(pEnableCalcAllPhiAsNoNegative->GetNewBoolValue(param));

	GateActorMessenger::SetNewValue(command ,param);
}

#endif
