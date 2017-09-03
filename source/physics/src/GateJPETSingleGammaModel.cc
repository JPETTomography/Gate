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
 *  @file GateJPETSingleGammaMode.cc
 */

#include "GateJPETSingleGammaModel.hh"
#include "TLorentzVector.h"
#include "G4Electron.hh"
#include "G4PhysicalConstants.hh"
#include "GateConstants.hh"
#include <cmath>

GateJPETSingleGammaModel* GateJPETSingleGammaModel::ptrJPETSingleGammaModel=0;
GateJPETSingleGammaModel::GateJPETSingleGammaModel()
{
	G4cout <<"GateJPETSingleGammaModel initialization.\n";
	SetParticlesNumber(1);
	GateJPETSourceManager::GetInstance()->AddGammaSourceModel(this);
}

GateJPETSingleGammaModel::~GateJPETSingleGammaModel()
{
}

void GateJPETSingleGammaModel::GetGammaParticles(std::vector<G4PrimaryParticle*>& particles)
{
	G4PrimaryParticle* particle = particles[0];
	particle->SetPolarization(GetPolarization(particle->GetMomentumDirection()));
}

G4String GateJPETSingleGammaModel::GetModelName()
{
	return "SGJPET";
}

GateJPETSingleGammaModel* GateJPETSingleGammaModel::GetInstance()
{
	if(!ptrJPETSingleGammaModel){
		ptrJPETSingleGammaModel = new GateJPETSingleGammaModel;
	}

	return ptrJPETSingleGammaModel;
}

