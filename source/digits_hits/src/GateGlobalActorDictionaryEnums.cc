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
 *  @file GateGlobalActorDictionaryEnums.cc
 */

#include "./GateGlobalActorDictionaryEnums.hh"
ClassImp(GateGlobalActorDictionaryEnums)

GateGlobalActorDictionaryEnums::GateGlobalActorDictionaryEnums() {}

GateGlobalActorDictionaryEnums::GammaSourceModel GateGlobalActorDictionaryEnums::getGammaSourceModel( const int& value )
{
 switch ( value )
 {
  case 0:
   return GateGlobalActorDictionaryEnums::GammaSourceModel::Unknown;
  case 1:
   return GateGlobalActorDictionaryEnums::GammaSourceModel::Single;
  case 2:
   return GateGlobalActorDictionaryEnums::GammaSourceModel::ParaPositronium;
  case 3:
   return GateGlobalActorDictionaryEnums::GammaSourceModel::OrthoPositronium;
  case 4:
   return GateGlobalActorDictionaryEnums::GammaSourceModel::ParaPositroniumAndPrompt;
  case 5:
   return GateGlobalActorDictionaryEnums::GammaSourceModel::OrthoPositroniumAndPrompt;
  case 6:
   return GateGlobalActorDictionaryEnums::GammaSourceModel::Other;
  default:
   return GateGlobalActorDictionaryEnums::GammaSourceModel::Unknown;
 };
}

GateGlobalActorDictionaryEnums::GammaKind GateGlobalActorDictionaryEnums::getGammaKind( const int& value )
{
 switch ( value )
 {
  case 0:
   return GateGlobalActorDictionaryEnums::GammaKind::GammaUnknown;
  case 1:
   return GateGlobalActorDictionaryEnums::GammaKind::GammaSingle;
  case 2:
   return GateGlobalActorDictionaryEnums::GammaKind::GammaPrompt;
  case 3:
   return GateGlobalActorDictionaryEnums::GammaKind::GammaFromParaPositronium;
  case 4:
   return GateGlobalActorDictionaryEnums::GammaKind::GammaFromOrthoPositronium;
  case 5:
   return GateGlobalActorDictionaryEnums::GammaKind::GammaFromOtherModel;
  default:
   return GateGlobalActorDictionaryEnums::GammaKind::GammaUnknown;
 };
}

GateGlobalActorDictionaryEnums::HitKind GateGlobalActorDictionaryEnums::getHitKind ( const int& value )
{
 switch ( value )
 {
  case 0:
   return GateGlobalActorDictionaryEnums::HitKind::HitUnknown;
  case 1:
   return GateGlobalActorDictionaryEnums::HitKind::HitNotMerged;
  case 2:
   return GateGlobalActorDictionaryEnums::HitKind::HitMerged;
  default:
   return GateGlobalActorDictionaryEnums::HitKind::HitUnknown;
 };
}
