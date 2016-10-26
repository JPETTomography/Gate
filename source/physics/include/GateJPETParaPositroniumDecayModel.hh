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
