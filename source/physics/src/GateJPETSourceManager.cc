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
#include "GateJPETSourceManager.hh"

GateJPETSourceManager* GateJPETSourceManager::ptrSingletonJPETSourceManager = 0;

GateJPETSourceManager::GateJPETSourceManager()
{
	mGammaSourceModelsNames = "";
}

GateJPETSourceManager::~GateJPETSourceManager()
{
}

GateJPETSourceManager* GateJPETSourceManager::GetInstance()
{
	if(!ptrSingletonJPETSourceManager){
		ptrSingletonJPETSourceManager = new GateJPETSourceManager;
		G4cout <<"GateJPETSourceManager initialized.\n";
	}

	return ptrSingletonJPETSourceManager;
}

void GateJPETSourceManager::AddGammaSourceModel(GateGammaSourceModel* model)
{
	if(model){
		if(!GetGammaSourceModelByName(model->GetModelName())){
			G4cout <<"GateJPETSourceManager::AddGammaSourceModel - Added model :  '"<<model->GetModelName()<<"'\n";
			mGammaSourceModelsNames += "<"+model->GetModelName()+">";
			mGammaSourceModels.emplace(model->GetModelName(),model);
		}
	}else {
		G4cout <<"GateJPETSourceManager::AddGammaSourceModel - null pointer to GateGammaSourceModel model.\n";
	}
}

GateGammaSourceModel* GateJPETSourceManager::GetGammaSourceModelByName(const G4String& modelName)
{
	std::map<G4String,GateGammaSourceModel* >::iterator found = mGammaSourceModels.find(modelName);
	if(found != mGammaSourceModels.end()){
		return found->second;
	}
	return 0;
}

G4String GateJPETSourceManager::GetGammaSourceModelsNames()
{
	return mGammaSourceModelsNames;
}
