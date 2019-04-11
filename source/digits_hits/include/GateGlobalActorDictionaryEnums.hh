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
 *  @file GateGlobalActorDictionaryEnums.hh
 */

#ifndef GateGlobalActorDictionaryEnums_hh
#define GateGlobalActorDictionaryEnums_hh
#include <TObject.h>


class GateGlobalActorDictionaryEnums: public TObject {
public:
  GateGlobalActorDictionaryEnums();

  enum GammaSourceModel
  {
   Unknown = 0,
   Single = 1,
   ParaPositronium = 2,
   OrthoPositronium = 3,
   ParaPositroniumAndPrompt = 4,
   OrthoPositroniumAndPrompt = 5,
   Other = 6
  };

  static GammaSourceModel getGammaSourceModel( const int& value );

  enum GammaKind
  {
   GammaUnknown = 0,
   GammaSingle = 1,
   GammaPrompt = 2,
   GammaFromParaPositronium = 3,
   GammaFromOrthoPositronium = 4,
   GammaFromOtherModel = 5
  };

  static GammaKind getGammaKind( const int& value );

  enum HitKind
  {
   HitUnknown = 0,
   HitNotMerged = 1,
   HitMerged = 2
  };

  static HitKind getHitKind( const int& value );

  ClassDef(GateGlobalActorDictionaryEnums,1)
};
#endif /*  !GateGlobalActorEnums_hh */
