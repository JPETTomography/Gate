/**
 *  @copyright Copyright 2019 The J-PET Gate Authors. All rights reserved.
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
 *  @file Dictionaries.h
 */
#ifndef Dictionaries_h
#define Dictionaries_h

#include "GateGlobalActorDictionariesLinkDef.hh"
#include "GateGlobalActorDictionaryEnums.hh"
#include "GateGlobalActorDictionaryEvent.hh"
#include "GateGlobalActorDictionaryHit.hh"
#include "GateGlobalActorDictionaryTrack.hh"

namespace Dictionaries
{
 typedef GateGlobalActorDictionaryEnums Enums;
 typedef GateGlobalActorDictionaryEvent Event;
 typedef GateGlobalActorDictionaryTrack Track;
 typedef GateGlobalActorDictionaryHit Hit;
};

#endif
