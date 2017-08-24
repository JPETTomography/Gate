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
 *  @file GateJPETSource.cc
 */

#include "GateJPETSource.hh"
#include "G4Event.hh"
#include "GateJPETSourceManager.hh"
#include "G4PrimaryParticle.hh"
#include "GateBackToBack.hh"

GateJPETSource::GateJPETSource(G4String name) : GateVSource(name), ptrGammaSourceModel(0)
{
	GateJPETModels::InitModels();
}

GateJPETSource::~GateJPETSource()
{
}

G4int GateJPETSource::GeneratePrimaries(G4Event* event)
{
	if (ptrGammaSourceModel) {
		GateVSource::SetParticleTime( GetTime() );
		//Then we set number of particles which will be generated
		GateVSource::SetNumberOfParticles( ptrGammaSourceModel->GetParticlesNumber() );
		//First we attach our event volume
		GateVSource::GeneratePrimaryVertex( event );

		std::vector<G4PrimaryParticle*> particles;
		particles.resize(ptrGammaSourceModel->GetParticlesNumber());
		for(int particleIndex=0; particleIndex<ptrGammaSourceModel->GetParticlesNumber(); ++particleIndex){
			particles[particleIndex] = event->GetPrimaryVertex(0)->GetPrimary( particleIndex );
		}
		// And finally we generate particles
		ptrGammaSourceModel->GetGammaParticles( particles );

	} else {
		if (InitModel()) {
			return GeneratePrimaries(event);
		} else {
			G4String commands = GateJPETSourceManager::GetInstance()->GetGammaSourceModelsNames();

			if (commands.size()>0) {
				GateError("Sorry, I don't know the source type '"<< GetType() << "'. Known source types are "<<commands);
			} else {
				GateError("Sorry, I don't know the source type '"<< GetType() << "'. There are no definided types");
			}
		}
	}

	return 1;
}

bool GateJPETSource::InitModel()
{
	ptrGammaSourceModel = GateJPETSourceManager::GetInstance()->GetGammaSourceModelByName(GetType());
	if(ptrGammaSourceModel){
		ptrGammaSourceModel->SetLinearPolarizationAngle(GateVSource::GetLinearPolarizationAngle(), false);
		ptrGammaSourceModel->SetUnpolarizedGammaGeneration(GateVSource::GetUnpolarizedParticlesGenerating());
	}
	return ptrGammaSourceModel == 0 ? false : true;
}

