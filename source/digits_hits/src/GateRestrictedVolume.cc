/** ----------------------
  Copyright (C): OpenGATE Collaboration
  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/

#include "GateRestrictedVolume.hh"

#include "G4UnitsTable.hh"

#include "GateTools.hh"
#include <memory>

GateRestrictedVolume::GateRestrictedVolume(GatePulseProcessorChain* itsChain, const G4String& itsName) : GateVPulseProcessor(itsChain, itsName)
{
  m_messenger = std::unique_ptr<GateRestrictedVolumeMessenger>(new GateRestrictedVolumeMessenger(this));
}

void GateRestrictedVolume::ProcessOnePulse(const GatePulse* inputPulse, GatePulseList& outputPulseList)
{
  if (!inputPulse)
  {
    if (nVerboseLevel > 1)
      G4cout << "[GateRestrictedVolume::ProcessOnePulse]: input pulse was null -> nothing to do\n\n";
    return;
  }

  if (((inputPulse->GetVolumeID()).GetBottomCreator())->GetObjectName() == "")
  {
    if (nVerboseLevel > 1)
      G4cout << "[GateRestrictedVolume::ProcessOneHit]: volume is null for " << inputPulse << " -> pulse ignored\n\n";
    return;
  }
  GatePulse* outputPulse = new GatePulse(*inputPulse);
  // Reject restriction mode -> Process Hits/Singles from all volumes belonds to the defined system excluding m_restrictedVolume
  if (((inputPulse->GetVolumeID()).GetBottomCreator())->GetObjectName() != m_restrictedVolume && m_restrictionMode == kReject)
  {
    outputPulseList.push_back(outputPulse);
    if (nVerboseLevel > 1)
      G4cout << "Copied pulse to output:\n" << inputPulse << Gateendl << Gateendl;
  }
  // Accept restriction mode -> Process Hits/Singles only from indicated volume (m_restrictedVolume) from the defined system
  else if (((inputPulse->GetVolumeID()).GetBottomCreator())->GetObjectName() == m_restrictedVolume && m_restrictionMode == kAccept)
  {
    outputPulseList.push_back(outputPulse);
    if (nVerboseLevel > 1)
      G4cout << "Copied pulse to output:\n" << inputPulse << Gateendl << Gateendl;
  }

  else
  {
    if (nVerboseLevel > 1)
      G4cout << "Ignored pulse:\n" << inputPulse << Gateendl << Gateendl;
  }
}

void GateRestrictedVolume::DescribeMyself(size_t indent)
{
  G4cout << GateTools::Indent(indent) << "Restricted volume: " << m_restrictedVolume << Gateendl;
  G4cout << GateTools::Indent(indent) << "Restriction mode: " << m_restrictionMode << Gateendl;
}
