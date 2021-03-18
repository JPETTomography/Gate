/** ----------------------
  Copyright (C): OpenGATE Collaboration
  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/


#ifndef GateRestrictedVolumeMessenger_h
#define GateRestrictedVolumeMessenger_h 1

#include "GatePulseProcessorMessenger.hh"

class G4UIcmdWithAString;

class GateRestrictedVolume;

class GateRestrictedVolumeMessenger: public GatePulseProcessorMessenger
{
  public:
    GateRestrictedVolumeMessenger(GateRestrictedVolume* itsRestrictedVolume);
    virtual ~GateRestrictedVolumeMessenger();

    inline void SetNewValue(G4UIcommand* aCommand, G4String aString);

    inline GateRestrictedVolume* RestrictedVolume()
      { return (GateRestrictedVolume*) GetPulseProcessor(); }

  private:
    G4UIcmdWithAString *restrictVolCmd = nullptr;
    G4UIcmdWithAString *restrictModeCmd = nullptr;
};

#endif
