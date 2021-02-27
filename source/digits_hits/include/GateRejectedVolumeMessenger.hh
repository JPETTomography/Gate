/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/


#ifndef GateRejectedVolumeMessenger_h
#define GateRejectedVolumeMessenger_h 1

#include "GatePulseProcessorMessenger.hh"

class G4UIcmdWithAString;

class GateRejectedVolume;

class GateRejectedVolumeMessenger: public GatePulseProcessorMessenger
{
  public:
    GateRejectedVolumeMessenger(GateRejectedVolume* itsRejectedVolume);
    virtual ~GateRejectedVolumeMessenger();

    inline void SetNewValue(G4UIcommand* aCommand, G4String aString);

    inline GateRejectedVolume* GetRejectedVolume()
      { return (GateRejectedVolume*) GetPulseProcessor(); }

  private:
    G4UIcmdWithAString   *rejectVolCmd = nullptr;
};

#endif
