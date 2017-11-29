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
 *  @file GlobalActorReader.hh
 */
#ifndef GLOBALACTORREADER_HH
#define GLOBALACTORREADER_HH
#include "Variable.hh"
#include "TTree.h"
#include "TFile.h"
#include "TVector3.h"

/** @About: This class is reader of data from GateGlobalActor.
 * Thanks to simple implementation you just need to declar variable and add initialization of variable in InitVariable() to obtain acces to data from file.
 * @Author: Mateusz Bała
 * @Email: bala.mateusz@gmail.com
*/
class GlobalActorReader
{
public:
    GlobalActorReader();
    ~GlobalActorReader();

    /** Function try open file and get acces to tree.
     * @param: file_name - ROOT file name
     * @return: false - when file does not exist, tree does not exist or tree is empty, otherwise true
    */
    bool LoadFile(const std::string& file_name);

    /** Function read data from tree. Each call of this function call next entity.
     * @return : false - when function reach maximal number of entries in tree, true - otherwise
    */
    bool Read();

    /** Resturn number number of entries in tree.
    */
    int GetEntriesNumber();

    /** Return number of initialized variables
    */
    int GetInitializedVariablesNumber();

    /** Reset counters and close file.
    */
    void Reset();

private:
    /** Try init variables. Use this function to read your variable.
     * You just need to add in this function : Notice(yourVariable.TryAttachToBranch(mTree, "yourBranchName");
    */
    void InitVariables();

    /** This function counter number of initialized variables.
     * @param: value - value from clas Variable function TryAttachToBranch
    */
    void Notice(const bool& value);

    /** Convert radians to degree
    */
    double deg(const double& angle_radians);

    //ROOT file
    TFile* pFile;
    //Tree from mFile
    TTree* pTree;
    //Number of initialized variables
    int mInitializedVariablesNumber;
    //Number of entries in tree
    int mEntriesNumber;
    //Current index of entry
    int mCurrentEntryIndex;

public:
/**GET-functions.
	This is only way how you return values;
*/
	/**Return volume (layer) name*/
	std::string GetVolumeName() const;
	/**Return scintilator translation vector*/
	TVector3 GetScintilatorPosition() const;
	/**Return event ID*/
	int GetEventID() const;
	/**Return trac ID*/
	int GetTrackID() const;
	/**Return particle energy before process (e.g. before scattering)*/
	double GetEnergyBeforeProcess() const;
	/**Return particle energy after process (e.g. after scattering)*/
	double GetEnergyAfterProcess() const;
	/**Return particle energy loss during process (e.g. during scattering)*/
	double GetEnergyLossDuringProcess() const;
	/**Return particle momemntum direction before process (e.g. before scattering)*/
	TVector3 GetMomentumDirectionBeforeProcess() const;
	/**Return particle momemntum direction after process (e.g. after scattering)*/
	TVector3 GetMomentumDirectionAfterProcess() const;
	/**Return proces position in lab coordinate system*/
	TVector3 GetProcessPosition() const;
	/**Return emission point from source*/
	TVector3 GetEmissionPointFromSource() const;
	/**Return particle momentum direction just after emission from source*/
	TVector3 GetEmissionMomentumDirectionFromSource() const;
	/**Return particle energy just after emission from source*/
	double GetEmissionEnergyFromSource() const;
	/**Return particle name*/
	std::string GetParticleName() const;
	/**Return particle PDG code*/
	int GetParticlePGDCoding() const;
	/**Return angle betwean particle momentum before and after process*/
	double GetProcessAngle() const;
	/**Return particle polarization before process*/
	TVector3 GetPolarizationBeforeProcess() const;
	/**Return particle polarization after process*/
	TVector3 GetPolarizationAfterProcess() const;
	/**Return process name*/
	std::string GetProcessName() const;

private:
/** VARIABLES
 * Declar here your variable by using template class Variable<yourType>
 * Please add description of your variable here
**/
    //Volume name (layer name)
    Variable<std::string> VolumeName;
    //Scintilator translation vector (if detector does not rotate it is equal scintilator centrum position
    Variable<TVector3> ScintilatorPosition;
    //Event ID
    Variable<int> EventID;
    //Track ID - each EventID has one or more tracks
    Variable<int> TrackID;
    //Particle energy before process (e.g. before scattering)
    Variable<double> EnergyBeforeProcess;
    //Particle energy after process (e.g. after scattering)
    Variable<double> EnergyAfterProcess;
    //Particle energy loss during process (e.g. during scattering)
    Variable<double> EnergyLossDuringProcess;
    //Particle momemntum direction before process (e.g. before scattering)
    Variable<TVector3> MomentumDirectionBeforeProcess;
    //Particle momemntum direction after process (e.g. after scattering)
    Variable<TVector3> MomentumDirectionAfterProcess;
    //Process position in lab coordinate system
    Variable<TVector3> ProcessPosition;
    //Emission point from source
    Variable<TVector3> EmissionPointFromSource;
    //Particle momentum direction just after emission from source
    Variable<TVector3> EmissionMomentumDirectionFromSource;
    //Particle energy just after emission from source
    Variable<double> EmissionEnergyFromSource;
    //Particle name
    Variable<std::string> ParticleName;
    //Particle PDG code
    Variable<int> ParticlePGDCoding;
    //Angle betwean particle momentum before and after process
    Variable<double> ProcessAngle;
    //Particle polarization before process
    Variable<TVector3> PolarizationBeforeProcess;
    //Particle polarization after process
    Variable<TVector3> PolarizationAfterProcess;
    //Process name
    Variable<std::string> ProcessName;

public:
/** EXTRA FUNCTIONS
 * If you need variable which is based on other variables lisetd in InitVariables().
 * Please add here your GetSthValue() e.g. GetComptonThetaValue();
 * Remember add decription of file
*/
    /** Return Compton scaterring angle (theta angle).
     * This is angle betwean prime and scattered gamma momentum direction.
     * Angle unit: degree
    */
    double GetComptonThetaValue();

    /** Return Compton phi angle.
     * This is angle betwean prime gamma polarization vector and scattered gamma momentum vector throw on
     * the plane perpendicular to prime gamma momentum direction.
     * Angle unit: degree
    */
   double GetComptonPhiValue();

};

#endif // GLOBALACTORREADER_HH
