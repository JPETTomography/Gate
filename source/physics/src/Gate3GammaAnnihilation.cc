/*----------------------
  Copyright (C): OpenGATE Collaboration

  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See GATE/LICENSE.txt for further details
  ----------------------*/

#include "Gate3GammaAnnihilation.hh"

// #include "G4PhysicalConstants.hh"
// #include "GateConstants.hh"

//-------------------------------------------------------------------------------------------------
Gate3GammaAnnihilation::Gate3GammaAnnihilation( GateVSource* source )
{
  m_source = source;
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
Gate3GammaAnnihilation::~Gate3GammaAnnihilation()
{
  ;
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
void Gate3GammaAnnihilation::Initialize()
{
  m_source->SetNumberOfParticles( 3 );
  m_source->SetParticleTime( m_source->GetTime() );
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
void Gate3GammaAnnihilation::GenerateVertex( G4Event* aEvent)
{


  m_source->GeneratePrimaryVertex( aEvent );
  G4PrimaryParticle* particle1 = aEvent->GetPrimaryVertex( 0 )->GetPrimary( 0 );
  G4PrimaryParticle* particle2 = aEvent->GetPrimaryVertex( 0 )->GetPrimary( 1 );
  G4PrimaryParticle* particle3 = aEvent->GetPrimaryVertex( 0 )->GetPrimary( 2 );

  G4ThreeVector gammaMom = particle1->GetMomentum();
  particle2->SetMomentum( gammaMom.x(),0, 0 );
  particle3->SetMomentum( 0, 0, gammaMom.z() );




}
//-------------------------------------------------------------------------------------------------
