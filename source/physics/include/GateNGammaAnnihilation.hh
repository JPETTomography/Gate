#ifndef GATENGAMMAANNIHILATION_HH
#define GATENGAMMAANNIHILATION_HH

#include<iostream>
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
