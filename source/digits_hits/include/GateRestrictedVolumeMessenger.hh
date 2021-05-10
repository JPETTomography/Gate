/** ----------------------
  Copyright (C): OpenGATE Collaboration
  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/

#ifndef GateRestrictedVolumeMessenger_h
#define GateRestrictedVolumeMessenger_h 1

#include "GatePulseProcessorMessenger.hh"
#include <memory>

class G4UIcmdWithAString;

class GateRestrictedVolume;

/** Author: Jakub Baran
 *  Email: jakubbaran92@gmail.com
 *  Organization: J-PET (http://koza.if.uj.edu.pl/pet/)
 *  About class: Messenger class of the GateRestrictedVolume
 **/

class GateRestrictedVolumeMessenger : public GatePulseProcessorMessenger
{
public:
  GateRestrictedVolumeMessenger(GateRestrictedVolume* itsRestrictedVolume);

  inline void SetNewValue(G4UIcommand* aCommand, G4String aString);

  inline GateRestrictedVolume* RestrictedVolume() { return (GateRestrictedVolume*)GetPulseProcessor(); }

private:
  G4String kAccept = "accept"; //!< Accept restriction mode string
  G4String kReject = "reject"; //!< Reject restriction mode string
  std::unique_ptr<G4UIcmdWithAString> restrictVolCmd;
  std::unique_ptr<G4UIcmdWithAString> restrictModeCmd;
};

#endif
