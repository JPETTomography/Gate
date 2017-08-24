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
 *  @file GateJPETSingleGammaModel.hh
 */

#ifndef GATEJPETSINGLEGAMMAMODEL_HH
#define GATEJPETSINGLEGAMMAMODEL_HH

#include "GateGammaSourceModel.hh"
#include "TGenPhaseSpace.h"
#include "GateJPETSourceManager.hh"

/**Author: Mateusz Bała
 * Email: bala.mateusz@gmail.com
 * About class: generate one gamma. This function is useful when you need test behave of something for just one gamma
 */
class GateJPETSingleGammaModel : public GateGammaSourceModel
{
	public:
		/** Destructor
		 * */
		virtual~GateJPETSingleGammaModel();
		/** Each particle is filled with data about momentum.
		 * @param: particles - list with initialized particles - without momentum information
		 * */
		virtual void GetGammaParticles(std::vector<G4PrimaryParticle*>& particles);
		/** Return model name.
		 * @return: model name - it's always simple string
		 * */
		virtual G4String GetModelName();
		/** If class object is not initialized this function do this and return pointer.
		 * @return: class object pointer
		 * */
		static GateJPETSingleGammaModel *GetInstance();
	private:
		/** Constructor
		 * */
		GateJPETSingleGammaModel();
		static GateJPETSingleGammaModel* ptrJPETSingleGammaModel;
};

#endif

