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
 *  @file GateJPETGammaPolarizationActorMessenger.hh
 */

#include "GateConfiguration.h"
#ifdef G4ANALYSIS_USE_ROOT
#ifndef GATEJPETACTORGAMMAPOLARIZATIONMESSENGER_HH
#define GATEJPETACTORGAMMAPOLARIZATIONMESSENGER_HH
#include "globals.hh"
#include "GateActorMessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class GateJPETGammaPolarizationActor;

/**
 * Actor messenger reated just for gamma quanta's polarization analysis in Compton process (for JPET use)
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateJPETGammaPolarizationActorMessenger : public GateActorMessenger
{
public:
	GateJPETGammaPolarizationActorMessenger(GateJPETGammaPolarizationActor* sensor);
	virtual ~GateJPETGammaPolarizationActorMessenger();
	void BuildCommands(G4String base);
	virtual void SetNewValue(G4UIcommand*, G4String);
protected:
	GateJPETGammaPolarizationActor* pActor;
	G4UIcmdWithAString* pEnableLogFile;
	G4UIcmdWithAString* pEnableDiagnosticFile;
	G4UIcmdWithABool* pEnableUsePhiFlag;
	G4UIcmdWithABool* pEnableUseThetaFlag;
	G4UIcmdWithADoubleAndUnit* pEnablePhiLimes;
	G4UIcmdWithADoubleAndUnit* pEnableThetaLimes;
	G4UIcmdWithADoubleAndUnit* pEnablePhiEpsilon;
	G4UIcmdWithADoubleAndUnit* pEnableThetaEpsilon;
	G4UIcmdWithABool* pEnableCalcAllPhiAsNoNegative;
	G4UIcmdWithABool* pEnableHistograms;
	G4UIcmdWithABool* pEnableTests;
	G4UIcmdWithADoubleAndUnit* pEnableAnglePrecisionInHistograms;
};
#endif //GATEJPETACTORGAMMAPOLARIZATIONMESSENGER_HH
#endif //G4ANALYSIS_USE_ROOT
