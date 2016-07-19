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

#include "TGenPhaseSpace.h"
#include "TRandom3.h"

class Gate3GammaAnnihilation
{
public:
  Gate3GammaAnnihilation( GateVSource* );
  ~Gate3GammaAnnihilation();

  void Initialize();
  void GenerateVertex( G4Event*);

private:
  GateVSource* m_source;
  TRandom3 m_random_gen;
  TGenPhaseSpace m_3_body_decay;
};

#endif
