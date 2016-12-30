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
 *  @file GateNGammaAnnihilation.cc
 */

#include "GateNGammaAnnihilation.hh"
#include "TLorentzVector.h"
#include <vector>
#include "G4Event.hh"
#include "G4Electron.hh"

GateNGammaAnnihilation::GateNGammaAnnihilation(GateVSource* sourcePtr, GateGammaSourceModel* positroniumDecaySource,GateGammaSourceModel* secondaryGammaSource)
: ptrSource(sourcePtr),ptrPositroniumDecaySource(positroniumDecaySource),ptrSecondaryGammaSource(secondaryGammaSource)
{
}

GateNGammaAnnihilation::~GateNGammaAnnihilation()
{
}

void GateNGammaAnnihilation::Initialize()
{
	ptrSource->SetParticleTime(ptrSource->GetTime());
	//We set on the basis of the gamma source how much will have been total generated gamma photons number - then we will have modified them (gamma photons) based on expectations from  GateGammaSourceModel's derived classes
	int partcilesNumber=0;
	if(ptrPositroniumDecaySource)
		partcilesNumber += ptrPositroniumDecaySource->GetParticlesNumber();
	if(ptrSecondaryGammaSource)
		partcilesNumber += ptrSecondaryGammaSource->GetParticlesNumber();
	ptrSource->SetNumberOfParticles(partcilesNumber);
}

void GateNGammaAnnihilation::GenerateVertex(G4Event* event)
{
	G4cout<<"genracja vertexu"<< Gateendl;
	ptrSource->GeneratePrimaryVertex( event );

	int firstSourceParticlesNumber=0;
	int secondSourceParticlesNumber=0;

	if(ptrPositroniumDecaySource){
		firstSourceParticlesNumber=ptrPositroniumDecaySource->GetParticlesNumber();
		if(firstSourceParticlesNumber>0){
			std::vector<G4PrimaryParticle*> particlesFromPositroniumDecay;
			particlesFromPositroniumDecay.resize(firstSourceParticlesNumber);
			for(int i=0; i<firstSourceParticlesNumber; ++i){
				particlesFromPositroniumDecay[i] = event->GetPrimaryVertex( 0 )->GetPrimary(i);
			}
			ptrPositroniumDecaySource->GetGammaParticles(particlesFromPositroniumDecay);
		}
	}
	if(ptrSecondaryGammaSource){
		secondSourceParticlesNumber=ptrSecondaryGammaSource->GetParticlesNumber();
		if(secondSourceParticlesNumber>0){
			std::vector<G4PrimaryParticle*> particlesFromSecondaryGammaSource;
			particlesFromSecondaryGammaSource.resize(secondSourceParticlesNumber);
			for(int i=0; i<secondSourceParticlesNumber; ++i){
				particlesFromSecondaryGammaSource[i] = event->GetPrimaryVertex( 0 )->GetPrimary(firstSourceParticlesNumber+i);
			}
			ptrSecondaryGammaSource->GetGammaParticles(particlesFromSecondaryGammaSource);
		}
	}
}
