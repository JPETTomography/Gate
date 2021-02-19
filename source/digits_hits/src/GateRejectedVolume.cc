/*----------------------
   Copyright (C): OpenGATE Collaboration

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/


#include "GateRejectedVolume.hh"

#include "G4UnitsTable.hh"

#include "GateRejectedVolumeMessenger.hh"
#include "GateTools.hh"

GateRejectedVolume::GateRejectedVolume(GatePulseProcessorChain* itsChain,
			       const G4String& itsName,
      	      	      	      	 G4String rejectedVolume)
  : GateVPulseProcessor(itsChain,itsName),
    m_rejectedVolume(rejectedVolume)
{
  m_messenger = new GateRejectedVolumeMessenger(this);
}




GateRejectedVolume::~GateRejectedVolume()
{
  delete m_messenger;
}



void GateRejectedVolume::ProcessOnePulse(const GatePulse* inputPulse,GatePulseList& outputPulseList)
{
  if (!inputPulse) {
    if (nVerboseLevel>1)
      	G4cout << "[GateRejectedVolume::ProcessOnePulse]: input pulse was null -> nothing to do\n\n";
    return;
  }
  //TODO
  // Compare strings I
  if (((inputPulse->GetVolumeID()).GetBottomCreator())->GetObjectName() == "") {
    if (nVerboseLevel>1)
      	G4cout << "[GateRejectedVolume::ProcessOneHit]: volume is null for " << inputPulse << " -> pulse ignored\n\n";
    return;
  }
  
  //TODO
  // Compare strings II
  if ( ((inputPulse->GetVolumeID()).GetBottomCreator())->GetObjectName() != m_rejectedVolume )
  {
    GatePulse* outputPulse = new GatePulse(*inputPulse);
    outputPulseList.push_back(outputPulse);
    if (nVerboseLevel>1)
      	G4cout << "Copied pulse to output:\n"
      	       << *outputPulse << Gateendl << Gateendl ;
  }
  else {
      if (nVerboseLevel>1)
      	G4cout << "Ignored pulse:\n"
      	       << *inputPulse << Gateendl << Gateendl ;
  }
}



void GateRejectedVolume::DescribeMyself(size_t indent)
{
  G4cout << GateTools::Indent(indent) << "Rejected volume: " << m_rejectedVolume << Gateendl;
}
