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
 *  @file GateJPETParaPositroniumDecayModel.hh
 */

#ifndef GATEJPETPARAPOSITRONIUMDECAYMODEL_HH
#define GATEJPETPARAPOSITRONIUMDECAYMODEL_HH

#include "GateGammaSourceModel.hh"
#include "TGenPhaseSpace.h"

/**Author: Mateusz Bała
 * Email: bala.mateusz@gmail.com
 * About class: generate two gamma particles from para Positronium decay
 */
class GateJPETParaPositroniumDecayModel : public GateGammaSourceModel
{
	public:
		GateJPETParaPositroniumDecayModel();
		virtual~GateJPETParaPositroniumDecayModel();
		virtual void GetGammaParticles(std::vector<G4PrimaryParticle*>& particles);
};

#endif
