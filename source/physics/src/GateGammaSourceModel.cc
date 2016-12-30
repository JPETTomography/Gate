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
 *  @file GateGammaSourceModel.cc
 */

#include "GateGammaSourceModel.hh"
GateGammaSourceModel::GateGammaSourceModel():mParticlesNumber(0)
{
	//We create neutral vector
	PositronMomentum = TVector3(0,0,0);
}

GateGammaSourceModel::~GateGammaSourceModel()
{
}

int GateGammaSourceModel::GetParticlesNumber() {
	return mParticlesNumber;
}

void GateGammaSourceModel::SetPositronMomentum(TVector3 boost){
	PositronMomentum = boost;
}
