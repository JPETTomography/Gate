/*----------------------
  Copyright (C): OpenGATE Collaboration

  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See GATE/LICENSE.txt for further details
  ----------------------*/

#include "Gate3GammaAnnihilation.hh"
#include "TLorentzVector.h"
#include "TGenPhaseSpace.h"
#include "TRandom3.h"

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
  m_source->SetNumberOfParticles( 1 );
  m_source->SetParticleTime( m_source->GetTime() );
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
void Gate3GammaAnnihilation::GenerateVertex( G4Event* aEvent)
{


  m_source->GeneratePrimaryVertex( aEvent );
  G4PrimaryParticle* particle1 = aEvent->GetPrimaryVertex( 0 )->GetPrimary( 0 );
  // G4PrimaryParticle* particle2 = aEvent->GetPrimaryVertex( 0 )->GetPrimary( 1 );
  // G4PrimaryParticle* particle3 = aEvent->GetPrimaryVertex( 0 )->GetPrimary( 2 );gammaMom.y()*gammaMom.y() + gammaMom.z()*gammaMom.z()

    Double_t mass_e = 511.0 ; //keV
    TRandom3 *  random_generator = new TRandom3();

  TLorentzVector vec_pozytonium(0.0, 0.0, 0.0, 2.0*mass_e);

  // masy cząstek potomnych =0 -> gammy
  Double_t mass_secondaries[3] = {0.0, 0.0, 0.0};

  TGenPhaseSpace event;
  event.SetDecay(vec_pozytonium, 3, mass_secondaries);

  Double_t weight;
  Double_t weight_max= event.GetWtMax()*pow(10,5);
  Double_t rwt;
  Double_t M_max = 7.65928*pow(10,-6);


        do{
            weight = event.Generate();
            weight = weight*pow((mass_e-event.GetDecay(0)->E())/(event.GetDecay(1)->E()*event.GetDecay(2)->E()),2) + pow((mass_e-event.GetDecay(1)->E())/(event.GetDecay(0)->E()*event.GetDecay(2)->E()),2) + pow((mass_e-event.GetDecay(2)->E())/(event.GetDecay(0)->E()*event.GetDecay(1)->E()),2);
            rwt = random_generator->Uniform(M_max*weight_max);
        }while( rwt > weight );

        TLorentzVector * g1 = event.GetDecay(0);
        TLorentzVector * g2 = event.GetDecay(1);
        TLorentzVector * g3 = event.GetDecay(2);


  G4ThreeVector gammaMom = particle1->GetMomentum();
  particle1 -> SetMomentum( g1->P(), 0, 0 );
  // particle1->SetMomentum( gammaMom.x(), 0, 0 );
  // particle2->SetMomentum( g2->Px()*g2->Px() + g2->Py()*g2->Py() + g2->Pz()*g2->Pz(),0, 0 );
  // particle3->SetMomentum( 0, 0, gammaMom.z() );




}
//-------------------------------------------------------------------------------------------------
