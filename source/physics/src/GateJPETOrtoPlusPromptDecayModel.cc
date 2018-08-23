/**
 *  @copyright Copyright 2017 The J-PET Gate Authors. All rights reserved.
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  @file GateJPETOrtoPlusPromptDecayModel.cc
 */

#include "GateJPETOrtoPlusPromptDecayModel.hh"
#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "G4Electron.hh"
#include "G4SystemOfUnits.hh"
GateJPETOrtoPlusPromptDecayModel* GateJPETOrtoPlusPromptDecayModel::ptrJPETOrtoPlusPromptDecayModel = 0;
GateJPETOrtoPlusPromptDecayModel::GateJPETOrtoPlusPromptDecayModel()
{
 G4cout << "GateJPETOrtoPlusPromptDecayModel initialization.\n";
 SetParticlesNumber( 4 );
 GateJPETSourceManager::GetInstance()->AddGammaSourceModel( this );
}

GateJPETOrtoPlusPromptDecayModel::~GateJPETOrtoPlusPromptDecayModel()
{
}

Double_t GateJPETOrtoPlusPromptDecayModel::calculate_mQED( Double_t mass_e, Double_t w1, Double_t w2, Double_t w3 )
{
 return pow( ( mass_e - w1 ) / ( w2 * w3 ), 2 ) + pow( ( mass_e-w2 ) / ( w1 * w3 ), 2 ) + pow( ( mass_e - w3 ) / ( w1 * w2 ) , 2 );
}

void GateJPETOrtoPlusPromptDecayModel::GetGammaParticles(std::vector<G4PrimaryParticle*>& particles)
{
 AddGammaFromDeexcitation( particles );
 AddGammasFromOrtoPositronium( particles );
}

void GateJPETOrtoPlusPromptDecayModel::AddGammasFromOrtoPositronium( std::vector<G4PrimaryParticle*>& particles )
{
 Double_t mass_e = 0.511 * MeV / 1000.0;//GeV - because TGenPhaseSpace work with GeV

 TLorentzVector pozytonium( 0.0, 0.0, 0.0, 2.0 * mass_e );

 // 3 gamma quanta mass
 Double_t mass_secondaries[3] = { 0.0, 0.0, 0.0 };

 TGenPhaseSpace m_3_body_decay;
 m_3_body_decay.SetDecay( pozytonium, 3, mass_secondaries );

 // Include dacay's weights
 Double_t weight;
 Double_t weight_max= m_3_body_decay.GetWtMax() * pow( 10, 5 );
 Double_t rwt;
 Double_t M_max = 7.65928 * pow( 10,-6 );
 m_random_gen.SetSeed(0);

 do {
  weight = m_3_body_decay.Generate();
  weight = weight*calculate_mQED( mass_e, m_3_body_decay.GetDecay( 0 )->E(), m_3_body_decay.GetDecay(1)->E(), m_3_body_decay.GetDecay(2)->E() );
  rwt = m_random_gen.Uniform( M_max * weight_max );
 } while( rwt > weight );

 int particles_number = GetParticlesNumber();

 for(int i = 1; i < particles_number; ++i){
  TLorentzVector partDir = *m_3_body_decay.GetDecay( i );
  partDir.Boost( GetPositronMomentum() );
  // "*1000.0" because GetDecay return momentum in GeV but Geant4 and Gate make calculation in MeV
  particles[ i ]->SetMomentum( ( partDir.Px() ) * 1000.0, (partDir.Py() ) * 1000.0, ( partDir.Pz() ) * 1000.0 ); 
  particles[ i ]->SetPolarization( GetPolarization( particles[ i ]->GetMomentumDirection() ) );
 }
}

void GateJPETOrtoPlusPromptDecayModel::AddGammaFromDeexcitation( std::vector<G4PrimaryParticle*>& particles )
{
 G4ThreeVector momentum_direction = GetRandomVectorOnSphere();
 G4double kinetic_energy = GetPromptGammaEnergy();
 G4ThreeVector gamma_polarization = GetPolarization( momentum_direction );

 particles[ 0 ]->SetMomentumDirection( momentum_direction );
 particles[ 0 ]->SetMass( 0.0 );
 particles[ 0 ]->SetKineticEnergy( kinetic_energy );
 particles[ 0 ]->SetPolarization( gamma_polarization );
}


G4ThreeVector GateJPETOrtoPlusPromptDecayModel::GetRandomVectorOnSphere()
{
 Double_t x = 0, y = 0, z = 0;
 GetRandomGenerator()->Sphere( x, y, z, 1.0 );
 return G4ThreeVector( x, y, z );
}

G4String GateJPETOrtoPlusPromptDecayModel::GetModelName()
{
 return "oPsAndPromptJPET";
}

GateJPETOrtoPlusPromptDecayModel* GateJPETOrtoPlusPromptDecayModel::GetInstance()
{
 if ( !ptrJPETOrtoPlusPromptDecayModel )
  ptrJPETOrtoPlusPromptDecayModel = new GateJPETOrtoPlusPromptDecayModel();
 return ptrJPETOrtoPlusPromptDecayModel;
}
