#include "GateLocalActor.hh"
#include "GateGlobalActor.hh"
#include "GateGlobalActorMessenger.hh"

GateLocalActor::GateLocalActor(G4String name, G4int depth):GateVActor(name, depth)
{
	pMessenger = new GateGlobalActorMessenger(this);
}

GateLocalActor::~GateLocalActor()
{
	delete pMessenger;
}

void GateLocalActor::Construct()
{
	GateVActor::Construct();
	EnableUserSteppingAction(true);
}

void GateLocalActor::UserSteppingAction(const GateVVolume *, const G4Step* step)
{
	GateGlobalActor::Instance()->NoticeStep(GetVolumeName(), step);
}

void GateLocalActor::SaveData()
{
	GateGlobalActor::Instance()->Write();
}

void GateLocalActor::ResetData()
{
	GateGlobalActor::Instance()->Reset();
}
