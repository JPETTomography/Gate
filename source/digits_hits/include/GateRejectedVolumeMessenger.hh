/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/


#ifndef GateRejectedVolumeMessenger_h
#define GateRejectedVolumeMessenger_h 1

#include "GatePulseProcessorMessenger.hh"

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;

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
    G4UIcmdWithAString   *rejectVolCmd;
};

#endif
