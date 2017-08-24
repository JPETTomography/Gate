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
 *  @file GateJPETModels.cc
 */

#include "GateJPETModels.hh"
/** Below add #include with your model class header file.
 * Please add new to #include comment with shot class description.
 * */
#include "GateJPETParaPositroniumDecayModel.hh" //Generate 2 gamma from PPs decay
#include "GateJPETOrtoPositroniumDecayModel.hh" //Generate 3 gamma from OPs decay
#include "GateJPETSingleGammaModel.hh" //Generate 1 gamma

GateJPETModels::GateJPETModels()
{
}

GateJPETModels::~GateJPETModels()
{
}

void GateJPETModels::InitModels()
{
	//Below add your class constructor call and description

	//Generate 2 gamma from PPs decay
	GateJPETParaPositroniumDecayModel::GetInstance();

	//Generate 3 gamma from OPs decay
	GateJPETOrtoPositroniumDecayModel::GetInstance();

	//Generate single gamma - useful for tests
	GateJPETSingleGammaModel::GetInstance();
}

