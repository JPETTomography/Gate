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
 *  @file GateGlobalActorMessenger.hh
 */
#ifndef GATEGLOBALACTORMESSENGER_HH
#define GATEGLOBALACTORMESSENGER_HH

#include "GateConfiguration.h"

//#include "GateLocalActor.hh"
#include "GateActorMessenger.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include <vector>

class GateLocalActor;

/**
 * @About class: This class is GateGlobalActor messenger. Each one GateLocalActor class has one speciment of this messenger for communicated with GateGlobalActor.
 * You only need to call once command from this class in your macro - for example if you have two LocalActors one attached to volume Vol1 and second to Vol2 you need only to call commands in first or
 * second LocalActors, otherwise values will be override by last call of command.
 * For add new command go to @AboutThisSection.
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateGlobalActorMessenger : public GateActorMessenger
{
public:
	/** Constructor. Do not care about this, leave it as it is.
	 * */
	GateGlobalActorMessenger(GateLocalActor* sensor);

	/** Destructor - do not call "delete" here - commands collection will delete all commands
	 * */
	virtual ~GateGlobalActorMessenger() override;

	/** This function is used by GateActorMessenger class family to build commands. Do not care about this method, leave it as it is.
	 * */
	virtual void BuildCommands(G4String base) override;

	/** This function is used by GateActorMessenger class family to interpret commands. Do not care about this method, leave it as it is.
	 * */
	virtual void SetNewValue(G4UIcommand*, G4String) override;

private:
	/** Use this function to add new bool type command.
	 * This function create new command basing on function parameters.
	 * @param: cmd_name - by this name you call this command from macro
	 * @param: cmd_guidance - here add description of this command
	 * @param: cmd_pointer - add here pointer which represent your command
	 * Example use: AddSimpleCmd("enableEmissionPoint", "Save information about emission point", pEnableEmissionPointCmd)
	 * */
	void AddSimpleCmd(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWithoutParameter** cmd_pointer);

	/** Use this function to add new bool type command.
	 * This function create new command basing on function parameters.
	 * @param: cmd_name - by this name you call this command from macro
	 * @param: cmd_guidance - here add description of this command
	 * @param: cmd_pointer - add here pointer which represent your command
	 * Example use:
	 * */
	void AddBoolCmd(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWithABool** cmd_pointer);

	/** Use this function to add new G4String type command.
	 * This function create new command basing on function parameters.
	 * @param: cmd_name - by this name you call this command from macro
	 * @param: cmd_guidance - here add description of this command
	 * @param: cmd_pointer - add here pointer which represent your command
	 * Example use: AddStringCmd("filterPartcileName", "Save data only for particle with this name", pFilterParticleName)
	 * */
	void AddStringCmd(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWithAString** cmd_pointer);

	/** Use this function to add new G4int type command.
	 * This function create new command basing on function parameters.
	 * @param: cmd_name - by this name you call this command from macro
	 * @param: cmd_guidance - here add description of this command
	 * @param: cmd_pointer - add here pointer which represent your command
	 * Example use: AddIntegerCmd("filterScatteringPerTrack", "Save data only for specific number of scatterings", pFilterScatteringPerTrack)
	 * */
	void AddIntegerCmd(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWithAnInteger** cmd_pointer);

	/** Use this function to add new G4double type command - use this only when value is dimensionless
	 * This function create new command basing on function parameters.
	 * @param: cmd_name - by this name you call this command from macro
	 * @param: cmd_guidance - here add description of this command
	 * @param: cmd_pointer - add here pointer which represent your command
	 * Example use:
	 * */
	void AddDoubleCmdWithoutUnit(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWithADouble** cmd_pointer);

	/** Use this function to add new G4double type command - this command is useful to prevent add command with bad units
	 * This function create new command basing on function parameters.
	 * @param: cmd_name - by this name you call this command from macro
	 * @param: cmd_guidance - here add description of this command
	 * @param: default_unit - defines the default unit and also it defines the category of the allowed units. Thus only the units categorized as the given default unit will be accepted. You can skip this by add "".
	 * @param: unit_candidates - defines the candidates of units. Units listed in the argument of this method must be separated by space(s). You can skip this by add "".
	 * @param: unit_category - defines the category of the units which will be accepted. The available categories can be found in G4SystemOfUnits.hh in global category. Only the units categorized in the given category
	 * are accepted as the second argument (unit). You can skip this by add "".
	 * @param: cmd_pointer - add here pointer which represent your command.
	 * @ATTENTION: You have to provide minimally one non empty string argument: default_unit, unit_candidates or unit_category.
	 * Example use:
	 * */
	void AddDoubleCmdWithUnit(const G4String& cmd_name, const G4String& cmd_guidance, const G4String& default_unit, const G4String& unit_candidates, const G4String& unit_category, G4UIcmdWithADoubleAndUnit** cmd_pointer);

	/** Use this function to add new G4ThreeVector type command - use this only when value is dimensionless
	 * This function create new command basing on function parameters.
	 * @param: cmd_name - by this name you call this command from macro
	 * @param: cmd_guidance - here add description of this command
	 * @param: cmd_pointer - add here pointer which represent your command
	 * Example use:
	 * */
	void Add3VectorCmdWithoutUnit(const G4String& cmd_name, const G4String& cmd_guidance, G4UIcmdWith3Vector** cmd_pointer);

	/** Use this function to add new G4ThreeVector type command - this command is useful to prevent add command with bad units
	 * This function create new command basing on function parameters.
	 * @param: cmd_name - by this name you call this command from macro
	 * @param: cmd_guidance - here add description of this command
	 * @param: default_unit - defines the default unit and also it defines the category of the allowed units. Thus only the units categorized as the given default unit will be accepted. You can skip this by add "".
	 * @param: unit_candidates - defines the candidates of units. Units listed in the argument of this method must be separated by space(s). You can skip this by add "".
	 * @param: unit_category - defines the category of the units which will be accepted. The available categories can be found in G4SystemOfUnits.hh in global category. Only the units categorized in the given category
	 * are accepted as the second argument (unit). You can skip this by add "".
	 * @param: cmd_pointer - add here pointer which represent your command
	 * @ATTENTION: You have to provide minimally one non empty string argument: default_unit, unit_candidates or unit_category
	 * Example use:
	 * */
	void Add3VectorCmdWithUnit(const G4String& cmd_name, const G4String& cmd_guidance, const G4String& default_unit, const G4String& unit_candidates, const G4String& unit_category, G4UIcmdWith3VectorAndUnit** cmd_pointer);

	/** Extract value basing on command and param value.
	 * @param: cmd_pointer - pointer to your command, is necessary to interpret param argument
	 * @param: parameter - string parameter, this string will by interpreted by command class
	 * @return: G4bool type value
	 * */
	G4bool GetCommandValue(G4UIcmdWithABool* cmd_pointer, const G4String& parameter) const;

	/** Extract value basing on command and param value.
	 * @param: cmd_pointer - pointer to your command, is necessary to interpret param argument
	 * @param: parameter - string parameter, this string will by interpreted by command class
	 * @return: G4String type value
	 * */
	G4String GetCommandValue(G4UIcmdWithAString* cmd_pointer, const G4String& parameter) const;

	/** Extract value basing on command and param value.
	 * @param: cmd_pointer - pointer to your command, is necessary to interpret param argument
	 * @param: parameter - string parameter, this string will by interpreted by command class
	 * @return: G4int type value
	 * */
	G4int GetCommandValue(G4UIcmdWithAnInteger* cmd_pointer, const G4String& parameter) const;
	/** Extract value basing on command and param value.
	 * @param: cmd_pointer - pointer to your command, is necessary to interpret param argument
	 * @param: parameter - string parameter, this string will by interpreted by command class
	 * @return: G4double type value
	 * */
	G4double GetCommandValue(G4UIcmdWithADouble* cmd_pointer, const G4String& param) const;

	/** Extract value basing on command and param value.
	 * @param: cmd_pointer - pointer to your command, is necessary to interpret param argument
	 * @param: parameter - string parameter, this string will by interpreted by command class
	 * @return: G4double type value
	 * */
	G4double GetCommandValue(G4UIcmdWithADoubleAndUnit* cmd_pointer, const G4String& parameter) const;

	/** Extract value basing on command and param value.
	 * @param: cmd_pointer - pointer to your command, is necessary to interpret param argument
	 * @param: parameter - string parameter, this string will by interpreted by command class
	 * @return: G4ThreeVector type value
	 * */
	G4ThreeVector GetCommandValue(G4UIcmdWith3Vector* cmd_pointer, const G4String& parameter) const;

	/** Extract value basing on command and param value.
	 * @param: cmd_pointer - pointer to your command, is necessary to interpret param argument
	 * @param: parameter - string parameter, this string will by interpreted by command class
	 * @return: G4ThreeVector type value
	 * */
	G4ThreeVector GetCommandValue(G4UIcmdWith3VectorAndUnit* cmd_pointer, const G4String& parameter) const;


	/** Create command path basing on command name, actor name and other properties.
	 * @param: cmd_name - your command name
	 * */
	G4String GetCmdPath(const G4String& cmd_name);

	/** Here add your G4bool type command (use command AddBoolCmd to add new one).
	 * */
	void BuildBoolCmds();

	/** Here add your G4String type command (use command AddStringCmd to add new one).
	* */
	void BuildStringCmds();

	/** Here add your G4inttype command (use command AddIntegerCmd to add new one).
	 * */
	void BuildIntegerCmds();

	/** Here add your G4double type command (use command AddDoubleCmdWithUnit or AddDoubleCmdWithoutUnit to add new one).
	* */
	void BuildDoubleCmds();

	/** Here add your G4ThreeVector type command (use command Add3VectorCmdWithoutUnit or Add3VectorCmdWithUnit to add new one).
	* */
	void Build3VectorCmds();

	/** Here add your G4bool and G4UIcmdWithoutParameter type command handling.
	 * @param: command - command pointer
	 * @param: parameter - potential value of command
	 * */
	G4bool SetNewBoolValue(G4UIcommand* command, G4String parameter);

	/** Here add your G4String type command handling.
	 * @param: command - command pointer
	 * @param: parameter - potential value of command
	 * */
	G4bool SetNewStringValue(G4UIcommand* command, G4String parameter);

	/** Here add your G4int type command handling.
	 * @param: command - command pointer
	 * @param: parameter - potential value of command
	 * */
	G4bool SetNewIntegerValue(G4UIcommand* command, G4String parameter);

	/** Here add your G4double type command handling.
	 * @param: command - command pointer
	 * @param: parameter - potential value of command
	 * */
	G4bool SetNewDoubleValue(G4UIcommand* command, G4String parameterg);

	/** Here add your G4ThreeVector type command handling.
	 * @param: command - command pointer
	 * @param: parameter - potential value of command
	 * */
	G4bool SetNew3VectorValue(G4UIcommand* command, G4String parameter);

protected:
	GateLocalActor* pActor;
	std::vector<G4UIcommand*> mCommands;
private:
	/** @AboutThisSection: If you want to add new command, you only have to add one declaration of your command pointer here - nothing more in this file.
	 *  @Remember: Please do not add description of commands here - this should have be done by using Add method in one of Build methods
	 * */

	//@CommandSection: G4UIcmdWithoutParameter
	G4UIcmdWithoutParameter* pEnableVolumeNameCmd;
	G4UIcmdWithoutParameter* pEnableScintilatorPositionCmd;
	G4UIcmdWithoutParameter* pEnableEventIDCmd;
	G4UIcmdWithoutParameter* pEnableTrackIDCmd;
	G4UIcmdWithoutParameter* pEnableEnergyBeforeProcessCmd;
	G4UIcmdWithoutParameter* pEnableEnergyAfterProcessCmd;
	G4UIcmdWithoutParameter* pEnableEnergyLossDuringProcessCmd;
	G4UIcmdWithoutParameter* pEnableMomentumDirectionBeforeProcessCmd;
	G4UIcmdWithoutParameter* pEnableMomentumDirectionAfterProcessCmd;
	G4UIcmdWithoutParameter* pEnableProcessPositionCmd;
	G4UIcmdWithoutParameter* pEnableEmissionPointFromSourceCmd;
	G4UIcmdWithoutParameter* pEnableEmissionMomentumDirectionFromSourceCmd;
	G4UIcmdWithoutParameter* pEnableEmissionEnergyFromSourceCmd;
	G4UIcmdWithoutParameter* pEnableParticleNameCmd;
	G4UIcmdWithoutParameter* pEnableParticlePGDCodingCmd;
	G4UIcmdWithoutParameter* pEnableProcessAngleCmd;
	G4UIcmdWithoutParameter* pEnablePolarizationBeforeProcessCmd;
	G4UIcmdWithoutParameter* pEnablePolarizationAfterProcessCmd;
	G4UIcmdWithoutParameter* pEnableProcessNameCmd;

	//@CommandSection: G4UIcmdWithAString
	G4UIcmdWithAString* pFileNameCmd;
	G4UIcmdWithAString* pFilterProcessNameCmd;
	G4UIcmdWithAString* pFilterParticleNameCmd;

	//@CommandSection: G4UIcmdWithAnInteger
	G4UIcmdWithAnInteger* pFilterParticlePDGCodeCmd;

	//@CommandSection: G4UIcmdWithADouble

	//@CommandSection: G4UIcmdWithADoubleAndUnit
	G4UIcmdWithADoubleAndUnit* pFilterProcessAngleCmd;

	//@CommandSection: G4UIcmdWith3Vector

	//@CommandSection: G4UIcmdWith3VectorAndUnit
	G4UIcmdWith3VectorAndUnit* pFilterEmissionPointCmd;

};

#endif
