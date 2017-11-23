/**
 *  @copyright Copyright 2018 The J-PET Gate Authors. All rights reserved.
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
 *  @file GlobalActorReader.cc
 */

#include "GlobalActorReader.hh"
#include<iostream>
#include "Variable.hh"
#include <random>
#include "TVector3.h"

GlobalActorReader::GlobalActorReader()
{
    mEntriesNumber = 0;
    mInitializedVariablesNumber = 0;
    mCurrentEntryIndex = 0;
}

GlobalActorReader::~GlobalActorReader()
{
    Reset();
}

double GlobalActorReader::deg(const double& angle_radians)
{
    return (angle_radians/M_PI) * 180.0;
}

bool GlobalActorReader::LoadFile(const std::string& file_name)
{
    std::string fname = file_name;
    if(!fname.size())
        return false;
    if(fname.rfind(".root") == std::string::npos)
        fname += ".root";
    pFile = new TFile(fname.c_str(), "READ");
    if(!pFile->IsOpen())
        return false;
    pTree = dynamic_cast<TTree*>(pFile->Get("GateGlobalActorTree"));
    if(!pTree)
        return false;
    InitVariables();
    mEntriesNumber = pTree->GetEntries();
    return mInitializedVariablesNumber > 0;
}

bool GlobalActorReader::Read()
{
    if(mCurrentEntryIndex < mEntriesNumber && pTree)
        pTree->GetEntry(mCurrentEntryIndex);
    else
        return false;

    ++mCurrentEntryIndex;
    return true;
}

int GlobalActorReader::GetEntriesNumber()
{
    return mEntriesNumber;
}

int GlobalActorReader::GetInitializedVariablesNumber()
{
    return mInitializedVariablesNumber;
}

void GlobalActorReader::Reset()
{
    mEntriesNumber = 0;
    mInitializedVariablesNumber = 0;
    mCurrentEntryIndex = 0;
}

void GlobalActorReader::InitVariables()
{
    if(!pTree)
    {
        std::cerr<<"pTree null poiter in InitVariables"<<std::endl;
        return;
    }

    Notice(VolumeName.TryAttachToBranch(&pTree, "VolumeName"));
    Notice(ScintilatorPosition.TryAttachToBranch(&pTree, "ScintilatorPosition"));
    Notice(EventID.TryAttachToBranch(&pTree, "EventID"));
    Notice(TrackID.TryAttachToBranch(&pTree, "TrackID"));
    Notice(EnergyBeforeProcess.TryAttachToBranch(&pTree, "EnergyBeforeProcess"));
    Notice(EnergyAfterProcess.TryAttachToBranch(&pTree, "EnergyAfterProcess"));
    Notice(EnergyLossDuringProcess.TryAttachToBranch(&pTree, "EnergyLossDuringProcess"));
    Notice(MomentumDirectionBeforeProcess.TryAttachToBranch(&pTree, "MomentumDirectionBeforeProcess"));
    Notice(MomentumDirectionAfterProcess.TryAttachToBranch(&pTree, "MomentumDirectionAfterProcess"));
    Notice(ProcessPosition.TryAttachToBranch(&pTree, "ProcessPosition"));
    Notice(EmissionPointFromSource.TryAttachToBranch(&pTree, "EmissionPointFromSource"));
    Notice(EmissionMomentumDirectionFromSource.TryAttachToBranch(&pTree, ""));
    Notice(EmissionEnergyFromSource.TryAttachToBranch(&pTree, "EmissionEnergyFromSource"));
    Notice(ParticleName.TryAttachToBranch(&pTree, "ParticleName"));
    Notice(ParticlePGDCoding.TryAttachToBranch(&pTree, "ParticlePGDCoding"));
    Notice(ProcessAngle.TryAttachToBranch(&pTree, "ProcessAngle"));
    Notice(PolarizationBeforeProcess.TryAttachToBranch(&pTree, "PolarizationBeforeProcess"));
    Notice(PolarizationAfterProcess.TryAttachToBranch(&pTree, "PolarizationAfterProcess"));
    Notice(ProcessName.TryAttachToBranch(&pTree, "ProcessName"));
}

void GlobalActorReader::Notice(const bool& value)
{
    if(value)
        ++mInitializedVariablesNumber;
}

double GlobalActorReader::GetComptonThetaValue()
{
    TVector3 k0 = MomentumDirectionBeforeProcess.GetValue();
    TVector3 k = MomentumDirectionAfterProcess.GetValue();
    return deg(k0.Angle(k));
}

double GlobalActorReader::GetComptonPhiValue()
{
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    //This is based on article "New Monte Carlo method for Compton and Rayleigh scattering by polarized gamma rays ,G.O. Depaola"

    TVector3 e0 = PolarizationBeforeProcess.GetValue();
    if(e0.Mag() == 0)
        return M_PI*(2.0*distribution(generator)-1.0);

    TVector3 k0 = MomentumDirectionBeforeProcess.GetValue();
    TVector3 k = MomentumDirectionAfterProcess.GetValue();

    double sinTheta = sin(k0.Angle(k));
    double phi = sinTheta == 0 ? M_PI*(2.0*distribution(generator)-1.0) : std::acos(k.Dot(e0)/sinTheta);

    if(k.Dot(k0.Cross(e0).Unit()) < 0)
        phi *= -1.0;

    return phi;
}
