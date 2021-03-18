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

#include "GateVPulseProcessor.hh"

class GateRestrictedVolumeMessenger;


/*! \class  GateRestrictedVolume
    \brief  Pulse-processor modelling a simple volume discriminator.

    - The method ProcessOnePulse of this class models a simple
      volume discriminator: any input pulse whose volume name is not as
      defined is copied into the output pulse-list.
      On the contrary, any input pulse whose name is the same as 
      specified is discarded.

      \sa GateVPulseProcessor
*/
class GateRestrictedVolume : public GateVPulseProcessor
{
  public:

    //! Constructs a new module attached to a GateDigitizer
    GateRestrictedVolume(GatePulseProcessorChain* itsChain, const G4String& itsName);
    //! Destructor
    virtual ~GateRestrictedVolume();

    //! Returns the restricted volume
    G4String GetRestrictedVolume() { return m_restrictedVolume; }

    //! Set the trestricted volume
    void SetRestrictedVolume(G4String val) { m_restrictedVolume = val; }

    //! Returns the restricted volume
    G4String GetRestrictionMode() { return m_restrictionMode; }

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
    void ProcessOnePulse(const GatePulse* inputPulse,GatePulseList&  outputPulseList);

  private:
    G4String m_restrictedVolume;                             //!< Rejected volume
    G4String m_restrictionMode;
    GateRestrictedVolumeMessenger *m_messenger = nullptr;    //!< Messenger
};


#endif
