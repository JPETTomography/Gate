#include "GateConfiguration.h"

#ifndef GATEJPETACTOR_HH
#define GATEJPETACTOR_HH

#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"

#include "GateVActor.hh"
#include "GateJPETActorMessenger.hh"

struct iaea_header_type;
struct iaea_record_type;

/**
 * Actor created for JEPT use
 * @author: Mateusz Bała
 * @email: bala.mateusz@gmail.com
 */
class GateJPETActor : public GateVActor
{
public:
	virtual ~GateJPETActor();
	// This macro initialize the CreatePrototype and CreateInstance
	FCT_FOR_AUTO_CREATOR_ACTOR(GateJPETActor)
	// Constructs the sensor
	virtual void Construct();
	// Callbacks
	virtual void UserSteppingAction(const GateVVolume *, const G4Step*);
	virtual void PreUserTrackingAction(const GateVVolume *, const G4Track*);
	virtual void BeginOfEventAction(const G4Event * e);

	/// Saves the data collected to the file
	virtual void SaveData();
	virtual void ResetData();

	///Setting functions - use it if want activate or deactivate saving some data

	/**
	 * Record data about x - position
	 * @param: enableXPosition - set true if u want have data of particle x-position
	 */
	void SetXPositionEnabled(bool enableXPosition);

	/**
	 * Record data about y - position
	 * @param: enableYPosition - set true if u want have data of particle y-position
	 */
	void SetYPositionEnabled(bool enableYPosition);

	/**
	 * Record data about z - position
	 * @param: enableZPosition - set true if u want have data of particle z-position
	 */
	void SetZPositionEnabled(bool enableZPosition);

	/**
	 * Redcord data about particle Name
	 * @param: enableParticleName
	 */
	void SetParticleNameEnabled(bool enableParticleName);

	/**
	 * Choose for which particle record data
	 * @param: particleName - set particle name from enabled names in G4 e.g. e-,e+,gamma, etc.
	 * @ATTENTION!: If particleName is incorrect any data will be generated
	 * @TIP: U can use more then once this function to add more particles to recored
	 * */
	void SetParticleEnabled(G4String particleName);

	/**
	 * Record data about particle trackID
	 * @param: enableTrackID - set true if u want have data about this
	 */
	void SetTrackIDEnabled(bool enableTrackID);

	/**
	 * Record data about particle eventID
	 * @param: enableEventID - set true if u want have data abut this
	 */
	void SetEventIDEnabled(bool enableEventID);

	/**
	 * Record data about particle ParentID
	 * @param: enableParentID - set true if u want have data abut this
	 */
	void SetParentIDEnabled(bool enableParentID);

	/**
	 * Record data about particle RunID
	 * @pram: enableRunID - set true if u want have data abut this
	 */
	void SetRunIDEnabled(bool enableRunID);

	/**
	 * Record data about particle kinetic energy Ekin
	 * @param: enableEkin - set true if u want have data abut this
	 */
	void SetEkinEnabled(bool enableEkin);

	/**
	 * Record data about partciel interaction time
	 * @param: enableInteractionTime - set true if u want have data abut this
	 */
	void SetInteractionTimeEnabled(bool enableInteractionTime);

	/**
	 * Record data about Process Name witch conected is with particle
	 * @param: enableProcessName - set true if u want have data abut this
	 */
	void SetProcessNameEnabled(bool enableProcessName);

	/**
	 * Record data about emission point
	 * @param: enableEmissionPoint - set true if u want have data abut this
	 */
	void SetEmissionPointEnabled(bool enableEmissionPoint);

	/**
	 * REcord data about primary energy
	 * @param: enablePrimaryEnergy - set true if u want have data abut this
	 */
	void SetPrimaryEnergy(bool enablePrimaryEnergy);

protected:
	GateJPETActor(G4String name, G4int depth=0);

	//Setting values showing which data record and which not - ATTENTION!: In defaul mode all bool setting are FALSE and vector is EMPTY
	/*Record data about x-position*/
	bool EnableXPosition;
	/*Record data about y-position*/
	bool EnableYPosition;
	/*Record data about z-position*/
	bool EnableZPosition;
	/*Record data about particle name*/
	bool EnableParticleName;
	/*Record data for particles from THIS list. ATTENTION: If this vector is empty class will be record data for every particles which fullfil conditions*/
	std::vector<G4String> ParticleNames;
	/*Record track ID*/
	bool EnableTrackID;
	/*Record Event ID*/
	bool EnableEventID;
	/*Record Parent ID*/
	bool EnableParentID;
	/*Record Run ID*/
	bool EnableRunID;
	/*Record Kinetic energy of particle*/
	bool EnableEkin;
	/*Record interaction time*/
	bool EnableInteractionTime;
	/*Record process name*/
	bool EnableProcessName;
	/*Record emission point*/
	bool EnableEmissionPoint;
	/*Record primary energy*/
	bool EnablePrimaryEnergy;

	//Values
	double positionX;
	double positionY;
	double positionZ;
	Char_t particleName[256];
	int trackID;
	int eventID;
	int runID;
	int parentID;
	double kinecticEnergy;
	double interactionTime;
	Char_t processName[256];
	double emissionPositionX;
	double emissionPositionY;
	double emissionPositionZ;
	double primaryEnergy;
	//Variables need to save data
	TString mFileType;
	TFile * pFile;
	TTree * pListeVar;
	GateJPETActorMessenger * pMessenger;

	//Tracing tools variables
	bool isFirstStep;

	//Special function just for this class
	/*
	 * Extract values setted by Set functions
	 * */
	void StandardExtractFunction(const G4Step *step);

};

MAKE_AUTO_CREATOR_ACTOR(JPETActor,GateJPETActor)

#endif /* end #define GATEJPETACTOR_HH */
