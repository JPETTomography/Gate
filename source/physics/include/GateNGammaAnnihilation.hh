#ifndef GATENGAMMAANNIHILATION_HH
#define GATENGAMMAANNIHILATION_HH

#include<iostream>
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
 *  @file GateNGammaAnnihilation.hh
 */

#include <stdarg.h>
#include "GateVSource.hh"
#include "TLorentzVector.h"
#include <vector>

#include "GateGammaSourceModel.hh"
#include "GateJPETParaPositroniumDecayModel.hh"
#include "GateJPETOrtoPositroniumDecayModel.hh"

/** Author: Mateusz Bała
 *  Email: bala.mateusz@gmail.com
 *  About class: This class provide generation of N gamma particles from any process of anihilation
 * */

class GateNGammaAnnihilation
{
	public:
		/** Constructor;
		 * @param: sourcePtr - source pointer,
		 * @param: positroniumDecaySource - ( positrnium decay source) add here model which simulate positronium decay (for.example oPs)
		 * @param: secondaryGammaSource - add here model which simulate secondaries particles from gamma decay
		  */
		GateNGammaAnnihilation(GateVSource* sourcePtr, GateGammaSourceModel* positroniumDecaySource,GateGammaSourceModel* secondaryGammaSource);
		~GateNGammaAnnihilation();

		void Initialize();

		/** Generate gamma particles and return them to event pointer.
		 * */
		void GenerateVertex( G4Event* event);

	private:
		GateVSource* ptrSource;
		GateGammaSourceModel* ptrPositroniumDecaySource;
		GateGammaSourceModel* ptrSecondaryGammaSource;

};


#endif
