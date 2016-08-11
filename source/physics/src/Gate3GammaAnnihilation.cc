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
  m_source->SetNumberOfParticles( 3 );
  m_source->SetParticleTime( m_source->GetTime() );
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
void Gate3GammaAnnihilation::GenerateVertex( G4Event* aEvent)
{


  m_source->GeneratePrimaryVertex( aEvent );
  G4PrimaryParticle* particle = aEvent->GetPrimaryVertex( 0 )->GetPrimary( 0 );
  G4PrimaryParticle* particle2 = aEvent->GetPrimaryVertex( 0 )->GetPrimary( 1 );
  G4PrimaryParticle* particle3 = aEvent->GetPrimaryVertex( 0 )->GetPrimary( 2 );



/*===================================================================
=            Begining of code provided by Daria Kamińska            =
===================================================================*/


  Double_t mass_e = 511.0 ; //keV


  // informacje o pozytonium
  TLorentzVector vec_pozytonium(0.0, 0.0, 0.0, 2.0*mass_e);

  // masy cząstek potomnych =0 -> gammy
  Double_t mass_secondaries[3] = {0.0, 0.0, 0.0};

  // TGenPhaseSpace - Utility class to generate n-body m_3_body_decay
  // TGenPhaseSpace m_3_body_decay;
  m_3_body_decay.SetDecay(vec_pozytonium, 3, mass_secondaries);

  // uwzględnienie wag rozpadu
  Double_t weight;
  Double_t weight_max= m_3_body_decay.GetWtMax()*pow(10,5);
  Double_t rwt;
  Double_t M_max = 7.65928*pow(10,-6);


    do{
        weight = m_3_body_decay.Generate();
        weight = weight*calculate_mQED(mass_e,m_3_body_decay.GetDecay(0)->E(),m_3_body_decay.GetDecay(1)->E(),m_3_body_decay.GetDecay(2)->E());
        rwt = m_random_gen.Uniform(M_max*weight_max);
    }while( rwt > weight );

    // get momenta
    TLorentzVector * g1 = m_3_body_decay.GetDecay(0);
    TLorentzVector * g2 = m_3_body_decay.GetDecay(1);
    TLorentzVector * g3 = m_3_body_decay.GetDecay(2);


/*==============================================================
=            End of code provided by Daria Kamińska            =
==============================================================*/


  particle->SetMomentum( (g1 -> Px())/1000.0, (g1 -> Py())/1000.0, (g1 -> Pz())/1000.0 );
  particle2->SetMomentum( (g2 -> Px())/1000.0, (g2 -> Py())/1000.0, (g2 -> Pz())/1000.0 );
  particle3->SetMomentum( (g3 -> Px())/1000.0, (g3 -> Py())/1000.0, (g3 -> Pz())/1000.0 );

  // Momenta changed to MeV (Daria is using keV)


}
//-------------------------------------------------------------------------------------------------
