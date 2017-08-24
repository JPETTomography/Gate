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
 *  @file GateJPETParaPositroniumDecayModel.cc
 */

#include "GateJPETParaPositroniumDecayModel.hh"
#include "TLorentzVector.h"
#include "G4Electron.hh"
#include "G4PhysicalConstants.hh"
#include "GateConstants.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>
GateJPETParaPositroniumDecayModel* GateJPETParaPositroniumDecayModel::ptrJPETParaPositroniumDecayModel=0;
GateJPETParaPositroniumDecayModel::GateJPETParaPositroniumDecayModel()
{
	G4cout <<"GateJPETParaPositroniumDecayModel initialization.\n";
	SetParticlesNumber(2);
	GateJPETSourceManager::GetInstance()->AddGammaSourceModel(this);
}

GateJPETParaPositroniumDecayModel::~GateJPETParaPositroniumDecayModel()
{
}

void GateJPETParaPositroniumDecayModel::GetGammaParticles(std::vector<G4PrimaryParticle*>& particles)
{

	Double_t mass_e = 0.511*MeV/1000.0;//GeV - because TGenPhaseSpace work with GeV

	TLorentzVector pozytonium(0.0, 0.0, 0.0, 2.0*mass_e);

	// 2 gamma quanta mass
	Double_t mass_secondaries[2] = {0.0, 0.0};

	TGenPhaseSpace m_2_body_decay;
	m_2_body_decay.SetDecay(pozytonium, 2, mass_secondaries);
	m_2_body_decay.Generate();

	int particles_number = GetParticlesNumber();
	for(int i=0; i<particles_number; ++i){
		TLorentzVector partDir = *m_2_body_decay.GetDecay(i);
		partDir.Boost(GetPositronMomentum());
		particles[i]->SetMomentum( (partDir.Px())*1000.0, (partDir.Py())*1000.0, (partDir.Pz())*1000.0 ); // "*1000.0" because GetDecay return momentum in GeV but Geant4 and Gate make calculation in MeV
		particles[i]->SetPolarization(GetPolarization(particles[i]->GetMomentumDirection()));
	}
}

G4String GateJPETParaPositroniumDecayModel::GetModelName()
{
	return "pPsJPET";
}

GateJPETParaPositroniumDecayModel* GateJPETParaPositroniumDecayModel::GetInstance()
{
	if(!ptrJPETParaPositroniumDecayModel){
		ptrJPETParaPositroniumDecayModel = new GateJPETParaPositroniumDecayModel;
	}

	return ptrJPETParaPositroniumDecayModel;
}
