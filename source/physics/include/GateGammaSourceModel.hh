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
 *  @file GateGammaSourceModel.hh
 */

#ifndef GATEGAMMASOURCEMDEL_HH
#define GATEGAMMASOURCEMDEL_HH

#include<iostream>
#include <vector>
#include "G4PrimaryParticle.hh"
#include "TLorentzVector.h"
#include "TVector3.h"

/** Author: Mateusz Bała
 *  Email: bala.mateusz@gmail.com
 *  About class: Clear abstract class which provide source of particles from any process
 *  			Your task is to wire class inherit this one and write working GetGammaParticles function.
 * */
class GateGammaSourceModel
{
	public:
		GateGammaSourceModel();
		virtual ~GateGammaSourceModel();
		/** This function's task is to generate gamma particles represented by TLorentzVector (for example from p-Ps decay or o-Ps decay, or other process)
		 * */
		virtual void GetGammaParticles(std::vector<G4PrimaryParticle*>& particles) = 0;
		// Remember - this number must be defined in constructor and return here
		int GetParticlesNumber();

		void SetPositronMomentum(TVector3 boost);

		int mParticlesNumber;
		TVector3 PositronMomentum;

	//protected:
	//	~GateGammaSourceModel();
};

#endif
