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
 *  @file GateGammaSourceModel.cc
 */

#include "GateGammaSourceModel.hh"
GateGammaSourceModel::GateGammaSourceModel()
{
	mParticlesNumber = 0;
	mPolarizationAngleInRadians = 0;
	mUseUnpolarizedGamma = false;
	mPositronMomentum = TVector3(0,0,0);
}

GateGammaSourceModel::~GateGammaSourceModel()
{
}


void GateGammaSourceModel::SetPositronMomentum(TVector3 boost)
{
	mPositronMomentum = boost;
}


G4ThreeVector GateGammaSourceModel::SetPerpendicularVector(G4ThreeVector& source_vector)
{
  G4double dx = source_vector.x();
  G4double dy = source_vector.y();
  G4double dz = source_vector.z();
  G4double x = dx < 0.0 ? -dx : dx;
  G4double y = dy < 0.0 ? -dy : dy;
  G4double z = dz < 0.0 ? -dz : dz;
  if (x < y)
    return x < z ? G4ThreeVector(-dy,dx,0) : G4ThreeVector(0,-dz,dy);
  else
    return y < z ? G4ThreeVector(dz,0,-dx) : G4ThreeVector(-dy,dx,0);
}

G4ThreeVector GateGammaSourceModel::GetPolarization(G4ThreeVector momentum_direction)
{
	/***
	 * Gamma polarization and direction are connected, and we can't set any value to polarization.
	 * Compton's scattering algorithm (e.g. G4LivermorePolarizationComptonModel.cc) prohibit polarization which is not orthogonal to gamma direction.
	 * Depends on how orthogonal is not this polarization Compton's algorithm generate two types of polarization: (p0 - prime polarization, d0 - gamma direction, p - new polarization)
	 * 1. If p0 is not orthogonal to d0 or p0 is zero vector (p0={0,0,0}) algorithm generate random orthogonal to d0 polarization p. Sometimes is good - e.g. we want obtain uniform phi angle
	 * distribution in Klein–Nishina cross-section plot  - in this situation always use zero vector in other case is always error and you will receive phi angle distribution in situation where it shoudn't be.
	 * 2. p0 is not exactly orthogonal to d0 - what mean dot product of d0 and p0 is not equal zero - in this situation algorithm  extract orthogonal part from p0 : p = p0 - p0.dot(d0)/d0.dot(d0) * d0;
	 * All above is only information for future users  - in this code I provide orthogonality of polarization as orthogonal product from gamma direction.
	 *
	 * More important information about Klein–Nishina algorithm here : polarization is threat as 3D vector in the Cartesian system - this isn't Stocke's vector.
	 * User can define only linear polarization by angle or lack of polarization.
	 * For define linear polarization use command : PolarizationAngleEnable
	 * For use unpolarized gamma use command : UnpolarizedGammaEnable
	 *
	 * Linear polarization is defined as a vector on plane orthogonal to gamma direction.
	 * */

	G4ThreeVector polarization = {0,0,0};
	G4ThreeVector d0 = momentum_direction.unit();
	if(!mUseUnpolarizedGamma)
	{
			G4ThreeVector a0,b0;
			a0 = SetPerpendicularVector(d0).unit();
			b0 = d0.cross(a0).unit();
			polarization = std::cos(mPolarizationAngleInRadians)*a0+std::sin(mPolarizationAngleInRadians)*b0;
			polarization.unit();
			polarization -= polarization.dot(d0) * d0;
	}
	return polarization;
}

void GateGammaSourceModel::SetLinearPolarizationAngle(double angle, bool is_degree)
{
	mPolarizationAngleInRadians = is_degree ? angle*(M_PI/180.0) : angle;
}

void GateGammaSourceModel::SetParticlesNumber(int particles_number)
{
	mParticlesNumber = particles_number;
}

int GateGammaSourceModel::GetParticlesNumber()
{
	return mParticlesNumber;
}

void GateGammaSourceModel::SetUnpolarizedGammaGeneration(bool use_unpolarized)
{
	mUseUnpolarizedGamma = use_unpolarized;
}

bool GateGammaSourceModel::GetIsUnpolarizedGammaGenerationInUse()
{
	return mUseUnpolarizedGamma;
}

TVector3 GateGammaSourceModel::GetPositronMomentum()
{
	return mPositronMomentum;
}
