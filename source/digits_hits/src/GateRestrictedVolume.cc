/** ----------------------
  Copyright (C): OpenGATE Collaboration
  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/

#include "GateRestrictedVolume.hh"

#include "G4UnitsTable.hh"

#include "GateRestrictedVolumeMessenger.hh"
#include "GateTools.hh"

GateRestrictedVolume::GateRestrictedVolume(GatePulseProcessorChain* itsChain,
                                       const G4String& itsName)
  : GateVPulseProcessor(itsChain, itsName)
{
  m_messenger = new GateRestrictedVolumeMessenger(this);
}

GateRestrictedVolume::~GateRestrictedVolume()
{
  delete m_messenger;
}

void GateRestrictedVolume::ProcessOnePulse(const GatePulse* inputPulse, GatePulseList& outputPulseList)
{
  if (!inputPulse) {
    if (nVerboseLevel>1)
      G4cout << "[GateRestrictedVolume::ProcessOnePulse]: input pulse was null -> nothing to do\n\n";
    return;
  }
  
  if (((inputPulse->GetVolumeID()).GetBottomCreator())->GetObjectName() == "") {
    if (nVerboseLevel>1)
      G4cout << "[GateRestrictedVolume::ProcessOneHit]: volume is null for " << inputPulse << " -> pulse ignored\n\n";
    return;
  }
  // Reject mode and the volume is the one specified
  if (((inputPulse->GetVolumeID()).GetBottomCreator())->GetObjectName() != m_restrictedVolume && m_restrictionMode == "reject")
  {
    G4cout << Gateendl << Gateendl << "DUPA3\n" << Gateendl << Gateendl ;
    outputPulseList.push_back(const_cast<GatePulse*>(inputPulse));
    if (nVerboseLevel>1)
      G4cout << "Copied pulse to output:\n" << inputPulse << Gateendl << Gateendl ;
  }
  // Accept mode and the volume is the one specified  
  else if (((inputPulse->GetVolumeID()).GetBottomCreator())->GetObjectName() == m_restrictedVolume && m_restrictionMode == "accept")
  {
    G4cout << Gateendl << Gateendl << "DUPA4\n" << Gateendl << Gateendl ;
    outputPulseList.push_back(const_cast<GatePulse*>(inputPulse));
    if (nVerboseLevel>1)
      G4cout << "Copied pulse to output:\n" << inputPulse << Gateendl << Gateendl ;
  }
  
  else {
    G4cout << Gateendl << Gateendl << "DUPA5\n" << Gateendl << Gateendl ;
    if (nVerboseLevel>1)
      G4cout << "Ignored pulse:\n" << inputPulse << Gateendl << Gateendl ;
  }

}

void GateRestrictedVolume::DescribeMyself(size_t indent)
{
  G4cout << GateTools::Indent(indent) << "Restricted volume: " << m_restrictedVolume << Gateendl;
  G4cout << GateTools::Indent(indent) << "Restriction mode: " << m_restrictionMode << Gateendl;
}
