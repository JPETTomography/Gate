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
 *  @file GateGlobalActorDictionaryEvent.hh
 */

#ifndef GateGlobalActorDictionaryEvent_hh
#define GateGlobalActorDictionaryEvent_hh
#include <TObject.h>
#include <vector>
#include "GateGlobalActorDictionaryTrack.hh"
#include "GateGlobalActorDictionaryEnums.hh"
#include "TVector3.h"

/**
 * @About class: This class represents single event.
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateGlobalActorDictionaryEvent: public TObject {
public:
  GateGlobalActorDictionaryEvent();
  unsigned int getTracksNumber() const;
  void clear();

  int fEventID = -1;
  std::vector<GateGlobalActorDictionaryTrack> fTracks;
  GateGlobalActorDictionaryEnums::GammaSourceModel fGammaSourceModel = GateGlobalActorDictionaryEnums::GammaSourceModel::Unknown;
  TVector3 fSourcePosition = TVector3( 0, 0, 0 );

  ClassDef(GateGlobalActorDictionaryEvent,1)
};
#endif /*  !GateGlobalActorEvent_hh */
