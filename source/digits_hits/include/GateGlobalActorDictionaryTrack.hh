/**
 *  @copyright Copyright 2019 The J-PET Framework Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file GateGlobalActorDictionaryTrack.hh
 */

#ifndef GateGlobalActorDictionaryTrack_hh
#define GateGlobalActorDictionaryTrack_hh
#include <TObject.h>
#include <vector>
#include "GateGlobalActorDictionaryHit.hh"
#include "GateGlobalActorDictionaryEnums.hh"
#include "TVector3.h"

/**
 * @About class: This class represents signle track. Information saved in this class include:
 * - list of hits ( each one track interaction with materials in simulation )
 * - information about particle's initial parameters: energy, momentum, polarization, name, PDG code, parentID
 * - gamma kind ( for example: is from pPs decay or is prompt gamma )
 * - information about compton scatterings: number of them, how many ( and they happened ) multiply sactterings in the same volume
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateGlobalActorDictionaryTrack: public TObject {
public:
  GateGlobalActorDictionaryTrack();
  int fTrackID = -1;
  std::vector<GateGlobalActorDictionaryHit> fHits;
  unsigned int getHitsNumber() const;

  //Properties from source
  double fInitialEnergy = 0.0;
  TVector3 fInitialMomentumDirection = TVector3( 0, 0, 0);
  TVector3 fInitialPolarization = TVector3( 0, 0, 0);
  GateGlobalActorDictionaryEnums::GammaKind fGammaKind = GateGlobalActorDictionaryEnums::GammaKind::GammaUnknown;
  std::string fParticleName = "";
  int fParticlePGDCoding = -1;
  int fParentID = -1;

  //Other
  unsigned int fScatteringsNumber = 0;
  unsigned int fMultiplicityScatteringsInTheSameVolume = 0;
  bool fAllHitsWithoutMultiplicityScatterings = true;
  double fTotalEnergyDeposition = 0.0;

  ClassDef(GateGlobalActorDictionaryTrack,1)
};
#endif /*  !GateGlobalActorTrack_hh */
