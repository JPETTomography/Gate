#include "GateJPETParaPositroniumDecayModel.hh"
#include "TLorentzVector.h"
#include "G4Electron.hh"
GateJPETParaPositroniumDecayModel::GateJPETParaPositroniumDecayModel()
{
	GateGammaSourceModel::mParticlesNumber = 2;
}

GateJPETParaPositroniumDecayModel::~GateJPETParaPositroniumDecayModel()
{
}

void GateJPETParaPositroniumDecayModel::GetGammaParticles(std::vector<G4PrimaryParticle*>& particles)
{
	Double_t mass_e = G4Electron::Definition()->GetPDGMass()/1000;//GeV

	// positronium
	TLorentzVector vec_pozytonium(0.0, 0.0, 0.0, 2.0*mass_e);

	// 2 gamma particles mass
	Double_t mass_secondaries[2] = {0.0, 0.0};

	TGenPhaseSpace m_2_body_decay;
	m_2_body_decay.SetDecay(vec_pozytonium, 2, mass_secondaries);
	m_2_body_decay.Generate();

	for(int i=0; i<GateGammaSourceModel::mParticlesNumber; ++i){
		TLorentzVector partDir = *m_2_body_decay.GetDecay(i);
		partDir.Boost(GateGammaSourceModel::mBoostVector);
		particles[i]->SetMomentum( (partDir.Px())/1000.0, (partDir.Py())/1000.0, (partDir.Pz())/1000.0 );
	}

}
