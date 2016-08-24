#include "GateJPETActor.hh"
//#ifdef G4ANALYSIS_USE_ROOT
#include "G4VProcess.hh"
#include "GateRunManager.hh"
#include "G4Run.hh"

#include "GateMiscFunctions.hh"
#include "GateObjectStore.hh"
#include "GateIAEAHeader.h"
#include "GateIAEARecord.h"
#include "GateIAEAUtilities.h"
#include "GateSourceMgr.hh"
#include "G4ParticleTable.hh"

GateJPETActor::GateJPETActor(G4String name, G4int depth):GateVActor(name, depth)
{
	GateDebugMessageInc("Actor", 4, "GateJPETActor() -- begin" << G4endl);
	pMessenger = new GateJPETActorMessenger(this);

	//For default set everything for FALSE
	 EnableXPosition = false;
	 EnableYPosition= false;
	 EnableZPosition= false;
	 EnableParticleName= false;
	 EnableTrackID= false;
	 EnableEventID= false;
	 EnableParentID= false;
	 EnableRunID= false;
	 EnableEkin= false;
	 EnableInteractionTime= false;
	 EnableProcessName= false;
	 EnableEmissionPoint = false;

	 mFileType = " ";

	 isFirstStep=true;

	 GateDebugMessageDec("Actor", 4, "GateJPETActor() -- end" << G4endl);
}

GateJPETActor::~GateJPETActor()
{
	GateDebugMessageInc("Actor", 4, "~GateJPETActor() -- begin" << G4endl);
	delete pMessenger;
	GateDebugMessageDec("Actor", 4, "~GateJPETActor() -- end" << G4endl);
}

void GateJPETActor::Construct()
{
	G4cout << "Wywolano JpetActor construct" << G4endl;
	//Calling functions from GateVActor
	GateVActor::Construct();
	EnableBeginOfRunAction(false);
	EnableBeginOfEventAction(false);
	EnablePreUserTrackingAction(true);
	EnableUserSteppingAction(true);
	G4String extension = getExtension(mSaveFilename);

	G4cout <<"root file name "<<mSaveFilename<< G4endl;
	G4cout <<"exten: "<< extension<<G4endl;
	//Yup -it's only for root files :P
	if(extension == "root"){
		mFileType = "rootFile";
	}else{
		GateError( "Unknow phase space file extension. Knowns extensions are .root"<< G4endl);
	}

	//OK - so what we want to save?
	if (mFileType == "rootFile"){
		pFile = new TFile(mSaveFilename, "RECREATE", "ROOT file for phase space", 9);
		pListeVar = new TTree("JPET", "JPET tree");

		if(EnableXPosition)
			pListeVar->Branch("ParticleXPosition", &positionX, "ParticleXPosition/D");
		if(EnableYPosition)
			pListeVar->Branch("ParticleYPosition", &positionY, "ParticleYPosition/D");
		if(EnableZPosition)
			pListeVar->Branch("ParticleZPosition", &positionZ, "ParticleZPosition/D");
		if(EnableParticleName)
			pListeVar->Branch("ParticleName", &particleName , "ParticleName/C");
		if(EnableTrackID)
			pListeVar->Branch("TrackID", &trackID, "TrackID/I");
		if(EnableEventID)
			pListeVar->Branch("EventID", &eventID, "EventID/I");
		if(EnableParentID)
			pListeVar->Branch("ParentID", &parentID, "ParentID/I");
		if(EnableRunID)
			pListeVar->Branch("RunID", &runID, "RunID/I");
		if(EnableEkin)
			pListeVar->Branch("ParticleKineticEnergy", &kinecticEnergy, "ParticleKineticEnergy/D");
		if(EnableInteractionTime)
			pListeVar->Branch("InteractionsTime", &interactionTime, "InteractionsTime/D");
		if(EnableProcessName)
			pListeVar->Branch("ProcessName", &processName , "ProcessName/C");
		if(EnableEmissionPoint){
			pListeVar->Branch("EmissionPointX", &emissionPositionX, "EmissionPointX/D");
			pListeVar->Branch("EmissionPointY", &emissionPositionY, "EmissionPointY/D");
			pListeVar->Branch("EmissionPointZ", &emissionPositionZ, "EmissionPointZ/D");
		}
		if(EnablePrimaryEnergy)
			pListeVar->Branch("ParticlePrimaryEnergy", &primaryEnergy, "ParticlePrimaryEnergy/D");

	}
}

void GateJPETActor::PreUserTrackingAction(const GateVVolume * /*v*/, const G4Track * t)
{
	isFirstStep = true;
	if(EnableEmissionPoint){
		emissionPositionX = t->GetVertexPosition().x();
		emissionPositionY = t->GetVertexPosition().y();
		emissionPositionZ = t->GetVertexPosition().z();
	}
}

void GateJPETActor::BeginOfEventAction(const G4Event *e)
{
	if(EnablePrimaryEnergy)
		primaryEnergy = e->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
}

void GateJPETActor::UserSteppingAction(const GateVVolume*, const G4Step *step)
{
	if(ParticleNames.size()==0){
		StandardExtractFunction( step);
	}else{
		std::vector<G4String>::iterator found = std::find(ParticleNames.begin(),ParticleNames.end(),step->GetTrack()->GetDefinition()->GetParticleName());
		if(found != ParticleNames.end()){
			StandardExtractFunction(step);
		}
	}
	 isFirstStep = false;
}

void GateJPETActor::StandardExtractFunction(const G4Step *step)
{
	if(EnableXPosition || EnableYPosition || EnableZPosition){
		G4ThreeVector localPosition = step->GetPostStepPoint()->GetPosition();
		if(EnableXPosition)
			positionX  = localPosition.x();
		if(EnableYPosition)
			positionY = localPosition.y();
		if(EnableZPosition)
			positionZ = localPosition.z();
	}

	if(EnableParticleName){
		G4String str = step->GetTrack()->GetDefinition()->GetParticleName();
		strcpy(particleName, str.c_str());
	}

	if(EnableTrackID)
		trackID = step->GetTrack()->GetTrackID();
	if(EnableEventID)
		eventID = GateRunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	if(EnableRunID)
		runID = GateRunManager::GetRunManager()->GetCurrentRun()->GetRunID();
	if(EnableParentID)
		parentID = step->GetTrack()->GetParentID();
	if(EnableEkin)
		kinecticEnergy = step->GetPostStepPoint()->GetKineticEnergy();
	if(EnableInteractionTime)
		interactionTime = step->GetPostStepPoint()->GetLocalTime();
	if(EnableProcessName) {
		G4String str = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
		strcpy(processName, str.c_str());
	}

	pListeVar->Fill();
}

void GateJPETActor::SaveData()
{
	GateVActor::SaveData();
	if (mFileType == "rootFile") {
		pFile = pListeVar->GetCurrentFile();
		pFile->Write();
	}
}

void GateJPETActor::ResetData() {
  if (mFileType == "rootFile") {
    pListeVar->Reset();
    return;
  }
}

void GateJPETActor::SetXPositionEnabled(bool enableXPosition)
{
	EnableXPosition = enableXPosition;
}


void GateJPETActor::SetYPositionEnabled(bool enableYPosition)
{
	EnableYPosition = enableYPosition;
}

void GateJPETActor::SetZPositionEnabled(bool enableZPosition)
{
	EnableZPosition = enableZPosition;
}

void GateJPETActor::SetParticleNameEnabled(bool enableParticleName)
{
	EnableParticleName = enableParticleName;
}

void GateJPETActor::SetParticleEnabled(G4String particleName)
{
	ParticleNames.push_back(particleName);
}

void GateJPETActor::SetTrackIDEnabled(bool enableTrackID)
{
	EnableTrackID = enableTrackID;
}


void GateJPETActor::SetEventIDEnabled(bool enableEventID)
{
	EnableEventID = enableEventID;
}

void GateJPETActor::SetParentIDEnabled(bool enableParentID)
{
	EnableParentID = enableParentID;
}

void GateJPETActor::SetRunIDEnabled(bool enableRunID)
{
	EnableRunID = enableRunID;
}

void GateJPETActor::SetEkinEnabled(bool enableEkin)
{
	EnableEkin = enableEkin;
}

void GateJPETActor::SetInteractionTimeEnabled(bool enableInteractionTime)
{
	EnableInteractionTime = enableInteractionTime;
}

void GateJPETActor::SetProcessNameEnabled(bool enableProcessName)
{
	EnableProcessName = enableProcessName;
}

void GateJPETActor::SetEmissionPointEnabled(bool enableEmissionPoint)
{
	EnableEmissionPoint = enableEmissionPoint;
}
void GateJPETActor::SetPrimaryEnergy(bool enablePrimaryEnergy)
{
	EnablePrimaryEnergy = enablePrimaryEnergy;
}
