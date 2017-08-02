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
 *  @file GateJPETSource.hh
 */
#ifndef GATEJPETSOURCE_HH
#define GATEJPETSOURCE_HH

#include <iomanip>
#include "globals.hh"
#include "G4VPrimaryGenerator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4ParticleMomentum.hh"
#include "TROOT.h"
#include "TFile.h"
#include "TH1D.h"
#include "TMath.h"
#include "TKey.h"
#include "GateVSource.hh"
#include <map>
#include "GateGammaSourceModel.hh"
#include "GateJPETModels.hh"

class GateJPETSource : public GateVSource
{
public:
	/** Constructor
	 * */
	GateJPETSource(G4String name);
	/** Destructor
	 * */
	virtual ~GateJPETSource();

	/** If program first do not choose gamma source model this function call InitModel;
	 * @param: event - event info
	 * */
	virtual G4int GeneratePrimaries(G4Event* event);

private:
	/** This function depends on user setting choose one correct model for simulation and associate with it pointer.
	 * */
	bool InitModel();
	// This is pointer which will be associated with your model
	GateGammaSourceModel* ptrGammaSourceModel;
};


#endif
