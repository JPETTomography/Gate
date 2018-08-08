
#ifndef GATE_JPET_SOURCE_MESSENGER_H
#define GATE_JPET_SOURCE_MESSENGER_H

#include "GateVSourceMessenger.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

class GateJPETSource;

class GateJPETSourceMessenger: public GateVSourceMessenger
{
 public:
  GateJPETSourceMessenger( GateJPETSource* source );
  ~GateJPETSourceMessenger();

  void SetNewValue( G4UIcommand* command, G4String newValue );

 private:
  void InitCommands();
  void DeleteCommands();

 protected:
  GateJPETSource* pSource = nullptr;
  G4UIcmdWithAnInteger* cmdSeedForRandomGenerator = nullptr;
  G4UIcmdWithADoubleAndUnit* cmdPromptGammaEnergy = nullptr;
  G4UIcmdWithADoubleAndUnit* cmdLinearPolarization = nullptr;
  G4UIcmdWithABool* cmdUseUnpolarizedParticles = nullptr;
};

#endif
