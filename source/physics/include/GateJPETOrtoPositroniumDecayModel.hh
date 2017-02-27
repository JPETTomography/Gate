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
 *  @fileGateJPETOrtoPositroniumDecayModel.hh
 */

#ifndef GATEJPETORTOPOSITRONIUMDECAYMODEL_HH
#define GATEJPETORTOPOSITRONIUMDECAYMODEL_HH

#include "GateGammaSourceModel.hh"
#include "TRandom3.h"
#include "GateJPETSourceManager.hh"

/**Author: Mateusz Bała
 * Email: bala.mateusz@gmail.com
 * Theorem author: Daria Kamińska
 * About class: Provide generation of 3 gamma from orto Positronium decay
 * */
class GateJPETOrtoPositroniumDecayModel : public GateGammaSourceModel
{
	public:
		/** Destructor
		 * */
		virtual ~GateJPETOrtoPositroniumDecayModel();
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
		static GateJPETOrtoPositroniumDecayModel* GetInstance();
	private:
		Double_t calculate_mQED(Double_t mass_e, Double_t w1, Double_t w2, Double_t w3);
		TRandom3 m_random_gen;
	private:
		/** Constructor
		 * */
		GateJPETOrtoPositroniumDecayModel();
		static GateJPETOrtoPositroniumDecayModel* ptrJPETOrtoPositroniumDecayModel;
};
#endif
