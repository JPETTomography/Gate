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

		void SetBoost(TVector3 boost);

		int mParticlesNumber;
		TVector3 mBoostVector;

	//protected:
	//	~GateGammaSourceModel();
};

#endif
