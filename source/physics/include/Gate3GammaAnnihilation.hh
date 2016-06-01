/*----------------------
  Copyright (C): OpenGATE Collaboration

  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See GATE/LICENSE.txt for further details
  ----------------------*/

#ifndef GATE3GAMMANNIHILATION_HH
#define GATE3GAMMANNIHILATION_HH

#include "G4Event.hh"

#include "GateVSource.hh"

class Gate3GammaAnnihilation
{
public:
  Gate3GammaAnnihilation( GateVSource* );
  ~Gate3GammaAnnihilation();

  void Initialize();
  void GenerateVertex( G4Event*);

private:
  GateVSource* m_source;
};

#endif
