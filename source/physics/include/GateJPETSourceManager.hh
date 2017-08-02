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
 *  @file GateJPETSourceManager.hh
 */
#ifndef GATEJPETSOURCEMANAGER_HH
#define GATEJPETSOURCEMANAGER_HH

#include<iostream>
#include<map>
#include "GateGammaSourceModel.hh"

/**Author: Mateusz Bała
 * Email: bala.mateusz@gmail.com
 * About class: The purpose of this class is to store information about available generation of gamma quanta models and sharing it.
 */
class GateJPETSourceManager{

public:
	~GateJPETSourceManager();

	static GateJPETSourceManager *GetInstance();

	void AddGammaSourceModel(GateGammaSourceModel* model);

	GateGammaSourceModel* GetGammaSourceModelByName(G4String modelName);

	G4String GetGammaSourceModelsNames();

private:
	GateJPETSourceManager();
	static GateJPETSourceManager* ptrSingletonJPETSourceManager;
	std::map<G4String,GateGammaSourceModel*> mGammaSourceModels;
	G4String mGammaSourceModelsNames;
};


#endif


