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
 *  @file GateGammaSourceModel.hh
 */

#ifndef GATEGAMMASOURCEMDEL_HH
#define GATEGAMMASOURCEMDEL_HH

#include<iostream>
#include <vector>
#include "G4PrimaryParticle.hh"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TRandom3.h"
#include <GateGammaModelPrimaryParticleInformation.hh>
//#include "GateJPETSourceManager.hh"
//#include "GateJPETSource.hh"

/** Author: Mateusz Bała
 *  Email: bala.mateusz@gmail.com
 *  About class: Clear abstract class which provide source of particles from any process.
 *  Your task is to write class which inherit this one and write working GetGammaParticles function.
 *  In addiction this class provide tool to generate linear polarization vectors (or zero polarization vectors).
 * */
class GateGammaSourceModel
{
 public:
  GateGammaSourceModel();
  virtual ~GateGammaSourceModel();
  /** Function fill each particle with data about momentum.
   * @param: particles - list with initialized particles - without momentum information
   * */
  virtual void GetGammaParticles(std::vector<G4PrimaryParticle*>& particles) = 0;

  /** Function set positron momentum.
   * @param: boost - positron boost
   * */
  void SetPositronMomentum(const TVector3& boost);

  TVector3 GetPositronMomentum();

  /** Function return model name.
   * @return: model name - it's always simple string
   * */
  virtual G4String GetModelName()=0;

  /** Function generate perpendicular vector to vector 'source_vector'.
   * @param: source_vector - vector with respect to which we generate a perpendicular vector
   * @return: perpendicular vector to vector 'source_vector'
   * */
  G4ThreeVector SetPerpendicularVector(const G4ThreeVector& source_vector);

  /** Function return polarization vector which is perpendicular to momentum direction.
   * @param: momentum_direction - gamma momentum direction with respect to which we generate a linear polarization vector
   * @return: linear polarization vector (this include zero vector {0,0,0} if unpolarized gamma generation is set.
   * */
  G4ThreeVector GetPolarization(const G4ThreeVector& momentum_direction);

  /** Function set angle for future linear polarization (in plane orthogonal to to gamma momentum direction)
   * @param: angle - angle value
   * @param: is_degree - if value of angle is in radians set this param as false
   * */
  void SetLinearPolarizationAngle(double angle, bool is_degree = true);

  /** Function set number gamma quanta which will be generate by children classes
   * @param: particle_number - number of gamma quanta which will be generate by model
   * */
  void SetParticlesNumber(int particles_number);

  /** Function return number of gamma quanta which will be generate by model
   * */
  int GetParticlesNumber();

  /** Function set generation of unpolarized gamma quanta if is needed
   * @param: use_unpolarized - set this true if you need unpolarized gamma
   * */
  void SetUnpolarizedGammaGeneration(bool use_unpolarized = false);

  /** Function inform if unpolarized gamma quanta generation is set.
   * @return: true - model generate unpolarized gamma, false - otherwise
   * */
  bool GetIsUnpolarizedGammaGenerationInUse();

  G4ThreeVector GetPerpendicularPolarizationToItsMomentumAndOtherPolarization(const G4ThreeVector& own_momentum_direction, const G4ThreeVector& other_polarization);

  void SetSeedForRandomGenerator( unsigned int seed );

  unsigned int GetSeedForRandomGenerator();

  void SetPromptGammaEnergy( double energy );

  double GetPromptGammaEnergy();

  TRandom3* GetRandomGenerator();

  GateGammaModelPrimaryParticleInformation* GetModelInfoForGamma( GateGammaModelPrimaryParticleInformation::GammaKind kind, const G4ThreeVector& polarization ) const;

  void SetGammaSourceModel( GateGammaModelPrimaryParticleInformation::GammaSourceModel model );

 private:
  G4ThreeVector calcPolarization( G4ThreeVector& d0, double angle_radians );

 private:
  //Number of gamma quanta which model generate
  int mParticlesNumber;
  //Positron momentum
  TVector3 mPositronMomentum;
  //Linear polarization angle in plane perpendicular to gamma momentum direction
  double mPolarizationAngleInRadians;
  bool mUseRandomAnglesForPolarizations;
  //When user want generate unpolarized gamma (i.e. zero polarization vector) this variable is true (by default false)
  bool mUseUnpolarizedGamma;

  unsigned int mSeedForRandomGenerator;
  TRandom3* ptrRandomGenerator = nullptr;
  double mPromptGammaEnergy;

  GateGammaModelPrimaryParticleInformation::GammaSourceModel mGammaSourceModel;
};

#endif
