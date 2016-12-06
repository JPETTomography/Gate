/**
 *  @copyright Copyright 2016 The J-PET Gate Authors. All rights reserved.
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
 *  @file GateJPETOrtoPositroniumDecayModel.cc
 */

#include "GateJPETOrtoPositroniumDecayModel.hh"
#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "G4Electron.hh"
GateJPETOrtoPositroniumDecayModel::GateJPETOrtoPositroniumDecayModel()
{
	GateGammaSourceModel::mParticlesNumber = 3;
}

GateJPETOrtoPositroniumDecayModel::~GateJPETOrtoPositroniumDecayModel()
{
}

Double_t GateJPETOrtoPositroniumDecayModel::calculate_mQED(Double_t mass_e, Double_t w1, Double_t w2, Double_t w3)
{
	return pow((mass_e-w1)/(w2*w3),2) + pow((mass_e-w2)/(w1*w3),2) + pow((mass_e-w3)/(w1*w2),2);
}

void GateJPETOrtoPositroniumDecayModel::GetGammaParticles(std::vector<G4PrimaryParticle*>& particles)
{
	Double_t mass_e = G4Electron::Definition()->GetPDGMass()*1000;//keV

	// positronium
	TLorentzVector vec_pozytonium(0.0, 0.0, 0.0, 2.0*mass_e);

	// 3 gamma particles mass
	Double_t mass_secondaries[3] = {0.0, 0.0, 0.0};

	// TGenPhaseSpace - Utility class to generate n-body m_3_body_decay
	TGenPhaseSpace m_3_body_decay;
	m_3_body_decay.SetDecay(vec_pozytonium, 3, mass_secondaries);

	// Include dacay's weights
	Double_t weight;
	Double_t weight_max= m_3_body_decay.GetWtMax()*pow(10,5);
	Double_t rwt;
	Double_t M_max = 7.65928*pow(10,-6);
	m_random_gen.SetSeed(0);

	do{
		weight = m_3_body_decay.Generate();
		weight = weight*calculate_mQED(mass_e,m_3_body_decay.GetDecay(0)->E(),m_3_body_decay.GetDecay(1)->E(),m_3_body_decay.GetDecay(2)->E());
		rwt = m_random_gen.Uniform(M_max*weight_max);
	}while( rwt > weight );

	for(int i=0; i<mParticlesNumber; ++i){
		TLorentzVector partDir = *m_3_body_decay.GetDecay(i);
		partDir.Boost(GateGammaSourceModel::PositronMomentum);
		particles[i]->SetMomentum( (partDir.Px())/1000.0, (partDir.Py())/1000.0, (partDir.Pz())/1000.0 );
	}
}
