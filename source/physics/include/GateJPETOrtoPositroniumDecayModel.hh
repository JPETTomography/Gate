#ifndef GATEJPETORTOPOSITRONIUMDECAYMODEL_HH
#define GATEJPETORTOPOSITRONIUMDECAYMODEL_HH

#include "GateGammaSourceModel.hh"
#include "TRandom3.h"

/**Author: Mateusz Bała
 * Email: bala.mateusz@gmail.com
 * Theorem author: Daria Kamińska
 * About class: Provide generation of 3 gamma from orto Positronium decay
 * */
class GateJPETOrtoPositroniumDecayModel : public GateGammaSourceModel
{
	public:
		GateJPETOrtoPositroniumDecayModel();
		virtual ~GateJPETOrtoPositroniumDecayModel();
		virtual void GetGammaParticles(std::vector<G4PrimaryParticle*>& particles);
	private:
		Double_t calculate_mQED(Double_t mass_e, Double_t w1, Double_t w2, Double_t w3);
	private:
		TRandom3 m_random_gen;


};

#endif
