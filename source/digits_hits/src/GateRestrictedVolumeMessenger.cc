/** ----------------------
  Copyright (C): OpenGATE Collaboration
  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/

#include "GateRestrictedVolumeMessenger.hh"

#include "GateRestrictedVolume.hh"

#include "G4UIcmdWithAString.hh"

GateRestrictedVolumeMessenger::GateRestrictedVolumeMessenger(GateRestrictedVolume* itsRestrictedVolume)
    : GatePulseProcessorMessenger(itsRestrictedVolume)
{
  G4String guidance;
  G4String cmdName;

  cmdName = GetDirectoryName() + "setRestrictedVolume";
  restrictVolCmd.reset(new G4UIcmdWithAString(cmdName, this));
  restrictVolCmd->SetGuidance("Set restricted volume for pulse-discrimination");

  cmdName = GetDirectoryName() + "setRestrictionMode";
  restrictModeCmd.reset(new G4UIcmdWithAString(cmdName, this));
  restrictModeCmd->SetGuidance("Set restriction mode, two are available: reject accept");
}

void GateRestrictedVolumeMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == restrictVolCmd.get())
  {
    RestrictedVolume()->SetRestrictedVolume(newValue);
  }

  else if (command == restrictModeCmd.get())
  {

    if (newValue == kAccept || newValue == kReject)
      RestrictedVolume()->SetRestrictionMode(newValue);

    else
    {
      G4cerr << "No match for '" << newValue << "' restriction mode.\n";
      G4cerr << "Candidates are: reject accept\n";

      G4String msg = "You must choose a restriction mode properly from two options: accept reject\n";
      G4Exception("GateRestrictedVolume::SetNewValue", "ProcessOnePulse", FatalException, msg);
    }
  }

  else
    GatePulseProcessorMessenger::SetNewValue(command, newValue);
}
