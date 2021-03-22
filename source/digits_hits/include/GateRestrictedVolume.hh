/** ----------------------
  Copyright (C): OpenGATE Collaboration
  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/

#ifndef GateRestrictedVolume_h
#define GateRestrictedVolume_h 1

#include "globals.hh"
#include <iostream>
#include <memory>

#include "GateRestrictedVolumeMessenger.hh"
#include "GateVPulseProcessor.hh"

class GateRestrictedVolumeMessenger;

/** Author: Jakub Baran
 *  Email: jakubbaran92@gmail.com
 *  Organization: J-PET (http://koza.if.uj.edu.pl/pet/)
 *  About class: Module enabled to either accept Hits/Singles
 *  from one specified volume (accept mode) or from all volumes
 *  belongs to the defined system exlucluding indicated volume
 *  (reject mode)
 **/

class GateRestrictedVolume : public GateVPulseProcessor
{
public:
  //! Constructs a new module attached to a GateDigitizer
  GateRestrictedVolume(GatePulseProcessorChain* itsChain, const G4String& itsName);

  //! Returns the restricted volume
  G4String GetRestrictedVolume() const { return m_restrictedVolume; }

  //! Set the restricted volume
  void SetRestrictedVolume(G4String val) { m_restrictedVolume = val; }

  //! Returns the restricted mode
  G4String GetRestrictionMode() const { return m_restrictionMode; }

  //! Set the restriction mode
  void SetRestrictionMode(G4String val) { m_restrictionMode = val; }

  //! Implementation of the pure virtual method declared by the base class GateClockDependent
  //! print-out the attributes specific of the module
  virtual void DescribeMyself(size_t indent);

protected:
  //! Implementation of the pure virtual method declared by the base class GateVPulseProcessor
  //! This methods processes one input-pulse
  //! It is is called by ProcessPulseList() for each of the input pulses
  //! The result of the pulse-processing is incorporated into the output pulse-list
  void ProcessOnePulse(const GatePulse* inputPulse, GatePulseList& outputPulseList) override;

private:
  G4String m_restrictedVolume;                                //!< Restricted volume
  G4String m_restrictionMode;                                 //!< Restriction mode
  G4String kAccept = "accept";                                //!< Accept restriction mode
  G4String kReject = "reject";                                //!< Reject restriction mode
  std::unique_ptr<GateRestrictedVolumeMessenger> m_messenger; //!< Messenger
};

#endif
