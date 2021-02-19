/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/


#include "GateRejectedVolumeMessenger.hh"

#include "GateRejectedVolume.hh"

#include "G4UIcmdWithAString.hh"

GateRejectedVolumeMessenger::GateRejectedVolumeMessenger(GateRejectedVolume* itsRejectedVolume)
    : GatePulseProcessorMessenger(itsRejectedVolume)
{
  G4String guidance;
  G4String cmdName;

  cmdName = GetDirectoryName() + "setRejectedVolume";
  rejectVolCmd = new G4UIcmdWithAString(cmdName,this);
  rejectVolCmd->SetGuidance("Set rejected volume for pulse-discrimination");
  
}


GateRejectedVolumeMessenger::~GateRejectedVolumeMessenger()
{
  delete rejectVolCmd;
}


void GateRejectedVolumeMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if ( command==rejectVolCmd )
    { GetRejectedVolume()->SetRejectedVolume(newValue); }
  else
    GatePulseProcessorMessenger::SetNewValue(command,newValue);
}
