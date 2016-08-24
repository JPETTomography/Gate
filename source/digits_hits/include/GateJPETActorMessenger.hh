#include "GateConfiguration.h"
#ifdef G4ANALYSIS_USE_ROOT
#ifndef GATEJPETACTORMESSENGER_HH
#define GATEJPETACTORMESSENGER_HH
#include "globals.hh"
#include "GateActorMessenger.hh"

class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class GateJPETActor;

/**
 * Actor messenger created for JEPT use
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateJPETActorMessenger : public GateActorMessenger
{
public:
	GateJPETActorMessenger(GateJPETActor* sensor);
	virtual ~GateJPETActorMessenger();

	void BuildCommands(G4String base);
	virtual void SetNewValue(G4UIcommand*, G4String);
protected:
	GateJPETActor* pActor;

	G4UIcmdWithABool* pEnableXPositionCmd;
	G4UIcmdWithABool* pEnableYPositionCmd;
	G4UIcmdWithABool* pEnableZPositionCmd;
	G4UIcmdWithABool* pEnableParticleNameCmd;
	G4UIcmdWithAString* pEnableParticleCmd;
	G4UIcmdWithABool* pEnableTrackIDCmd;
	G4UIcmdWithABool* pEnableEventIDCmd;
	G4UIcmdWithABool* pEnableParentIDCmd;
	G4UIcmdWithABool* pEnableRunIDCmd;
	G4UIcmdWithABool* pEnableEkinCmd;
	G4UIcmdWithABool* pEnableInteractionTimeCmd;
	G4UIcmdWithABool* pEnableProcessNameCmd;
	G4UIcmdWithABool* pEnableEmissionPointCmd;
	G4UIcmdWithABool* pEnablePrimaryEnergyCmd;
};
#endif //GATEJPETACTORMESSANGER_HH/
#endif //G4ANALYSIS_USE_ROOT
