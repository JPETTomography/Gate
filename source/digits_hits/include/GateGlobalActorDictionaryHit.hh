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
 *  @file GateGlobalActorDictionaryHit.hh
 */

#ifndef GateGlobalActorDictionaryHit_hh
#define GateGlobalActorDictionaryHit_hh
#include <TObject.h>
#include "TVector3.h"
#include <string>

#include "GateGlobalActorDictionaryEnums.hh"

/**
 * @About class: This class represents single particle interaction with the matter. Information saved to this file include all particles parameters changes during process and other useful varaibles.
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateGlobalActorDictionaryHit: public TObject {
public:
  GateGlobalActorDictionaryHit();

  int fHitID = -1;
  GateGlobalActorDictionaryEnums::HitKind fHitKind = GateGlobalActorDictionaryEnums::HitKind::HitUnknown;

  std::string fVolumeName = "";

  TVector3 fScintillatorPosition = TVector3( 0, 0, 0);

  double fEnergyBeforeProcess = 0.0;

  double fEnergyAfterProcess = 0.0;

  double fEnergyLossDuringProcess = 0.0;

  TVector3 fMomentumDirectionBeforeProcess = TVector3( 0, 0, 0);

  TVector3 fMomentumDirectionAfterProcess = TVector3( 0, 0, 0);

  TVector3 fProcessPosition = TVector3( 0, 0, 0);

  TVector3 fPolarizationBeforeProcess = TVector3( 0, 0, 0);

  TVector3 fPolarizationAfterProcess = TVector3( 0, 0, 0);

  std::string fProcessName = "";

  double fInteractionTime = 0.0;

  double fLocalTime = 0.0;

  ClassDef(GateGlobalActorDictionaryHit,1)
};
#endif /*  !GateGlobalActorTrack_hh */
