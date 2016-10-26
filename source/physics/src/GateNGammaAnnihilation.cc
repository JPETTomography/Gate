#include "GateNGammaAnnihilation.hh"
#include "TLorentzVector.h"
#include <vector>
#include "G4Event.hh"
#include "G4Electron.hh"

GateNGammaAnnihilation::GateNGammaAnnihilation(GateVSource* sourcePtr, GateGammaSourceModel* positroniumDecaySource,GateGammaSourceModel* secondaryGammaSource)
: ptrSource(sourcePtr),ptrPositroniumDecaySource(positroniumDecaySource),ptrSecondaryGammaSource(secondaryGammaSource)
{
}

GateNGammaAnnihilation::~GateNGammaAnnihilation()
{
}

void GateNGammaAnnihilation::Initialize()
{
	ptrSource->SetParticleTime(ptrSource->GetTime());
	//Ustalamy na podstawie otrzymanyź źródeł gamm ile będzie łącznie wygenerowanych gamm - potem będziemy je modyfikować zgodnie z oczekiwaniami klas pochodnych GateGammaSourceModel
	int partcilesNumber=0;
	if(ptrPositroniumDecaySource)
		partcilesNumber += ptrPositroniumDecaySource->GetParticlesNumber();
	if(ptrSecondaryGammaSource)
		partcilesNumber += ptrSecondaryGammaSource->GetParticlesNumber();
	ptrSource->SetNumberOfParticles(partcilesNumber);
}

void GateNGammaAnnihilation::GenerateVertex(G4Event* event)
{
	ptrSource->GeneratePrimaryVertex( event );
	int firstSourceParticlesNumber=0;
	int secondSourceParticlesNumber=0;

	if(ptrPositroniumDecaySource){
		firstSourceParticlesNumber=ptrPositroniumDecaySource->GetParticlesNumber();
		if(firstSourceParticlesNumber>0){
			std::vector<G4PrimaryParticle*> particlesFromPositroniumDecay;
			for(int i=0; i<firstSourceParticlesNumber; ++i){
				particlesFromPositroniumDecay[i] = event->GetPrimaryVertex( 0 )->GetPrimary(i);
			}
			ptrPositroniumDecaySource->GetGammaParticles(particlesFromPositroniumDecay);
		}
	}

	if(ptrSecondaryGammaSource){
		secondSourceParticlesNumber=ptrSecondaryGammaSource->GetParticlesNumber();
		if(secondSourceParticlesNumber>0){
			std::vector<G4PrimaryParticle*> particlesFromSecondaryGammaSource;
			for(int i=0; i<secondSourceParticlesNumber; ++i){
				particlesFromSecondaryGammaSource[i] = event->GetPrimaryVertex( 0 )->GetPrimary(firstSourceParticlesNumber+i);
			}
			ptrSecondaryGammaSource->GetGammaParticles(particlesFromSecondaryGammaSource);
		}
	}
}
