#include "GateGammaSourceModel.hh"
GateGammaSourceModel::GateGammaSourceModel():mParticlesNumber(0)
{
	//We create neutral vector
	mBoostVector = TVector3(0,0,0);
}

GateGammaSourceModel::~GateGammaSourceModel()
{
}

int GateGammaSourceModel::GetParticlesNumber() {
	return mParticlesNumber;
}

void GateGammaSourceModel::SetBoost(TVector3 boost){
	mBoostVector = boost;
}
