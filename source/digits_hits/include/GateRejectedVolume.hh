/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/

#ifndef GateRejectedVolume_h
#define GateRejectedVolume_h 1

#include "globals.hh"
#include <iostream>

#include "GateVPulseProcessor.hh"

class GateRejectedVolumeMessenger;

/*! \class  GateRejectedVolume
    \brief  Pulse-processor modelling a simple volume discriminator.

    - The method ProcessOnePulse of this class models a simple
      volume discriminator: any input pulse whose volume name is not as
      defined is copied into the output pulse-list.
      On the contrary, any input pulse whose name is the same as
      specified is discarded.

      \sa GateVPulseProcessor
*/
class GateRejectedVolume : public GateVPulseProcessor
{
public:
  //! Constructs a new module attached to a GateDigitizer
  GateRejectedVolume(GatePulseProcessorChain* itsChain, const G4String& itsName, G4String rejectedVolume);
  //! Destructor
  virtual ~GateRejectedVolume();

  //! Returns the rejected volume
  G4String GetRejectedVolume() { return m_rejectedVolume; }

  //! Set the threshold
  void SetRejectedVolume(G4String val) { m_rejectedVolume = val; }

  //! Implementation of the pure virtual method declared by the base class GateClockDependent
  //! print-out the attributes specific of the module
  virtual void DescribeMyself(size_t indent);

protected:
  //! Implementation of the pure virtual method declared by the base class GateVPulseProcessor
  //! This methods processes one input-pulse
  //! It is is called by ProcessPulseList() for each of the input pulses
  //! The result of the pulse-processing is incorporated into the output pulse-list
  void ProcessOnePulse(const GatePulse* inputPulse, GatePulseList& outputPulseList);

private:
  G4String m_rejectedVolume;                          //!< Rejected volume
  GateRejectedVolumeMessenger* m_messenger = nullptr; //!< Messenger
};

#endif