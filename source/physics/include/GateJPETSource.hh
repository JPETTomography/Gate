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
#include "GateJPETSourceMessenger.hh"

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
 virtual G4int GeneratePrimaries(G4Event* event) override;

 void SetSeedForRandomGenerator( unsigned int seed );

 void SetPromptGammaEnergy( double energy );

 /** Function set linear polarization angle for particle
  * @param: angle - angle value (degree unit)
  * */
 void SetLinearPolarizationAngle( double angle );

 /** Function set generation of unpolarized particles (what mean that particle has zero polarization vector {0,0,0})
  * @param: use_unpolarized - set true if you need unpolarized particles
  * */
 void SetUnpolarizedParticlesGenerating( bool use_unpolarized );

private:
 /** This function depends on user setting choose one correct model for simulation and associate with it pointer.
  * */
 bool InitModel();
 // This is pointer which will be associated with your model
 GateGammaSourceModel* ptrGammaSourceModel = nullptr;
 GateJPETSourceMessenger* pSourceMessenger = nullptr;
 unsigned int fSeedForRandomGenerator;
 double fPromptGammaEnergy;
 double fLinearPolarizationAngle;
 double fUseUnpolarizedParticles;
 

};


#endif
