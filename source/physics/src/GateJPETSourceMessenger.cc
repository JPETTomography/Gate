#include "GateJPETSourceMessenger.hh"
#include "GateJPETSource.hh"

GateJPETSourceMessenger::GateJPETSourceMessenger( GateJPETSource* source ) : GateVSourceMessenger( source )
{
 pSource = source;
 InitCommands();
}

GateJPETSourceMessenger::~GateJPETSourceMessenger() { DeleteCommands(); }

void GateJPETSourceMessenger::InitCommands()
{
 G4String cmdName = "";
 
 cmdName = GetDirectoryName() + "setSeedForRandomGenerator";
 cmdSeedForRandomGenerator = new G4UIcmdWithAnInteger( cmdName, this );
 cmdSeedForRandomGenerator->SetGuidance( "A seed for TRandom3 in gamma sources" );
 cmdSeedForRandomGenerator->SetParameterName( "seedRG", false );
 cmdSeedForRandomGenerator->SetRange( "seedRG>=0" );
 
 cmdName = GetDirectoryName() + "setPromptGammaEnergy";
 cmdPromptGammaEnergy = new G4UIcmdWithADoubleAndUnit( cmdName, this );
 cmdPromptGammaEnergy->SetGuidance( "Energy of prompt gamma" );

 cmdName = GetDirectoryName()+"setLinearPolarizationAngle";
 cmdLinearPolarization = new G4UIcmdWithADoubleAndUnit( cmdName, this );
 cmdLinearPolarization->SetGuidance( "Set linear polarization by given angle" );

 cmdName = GetDirectoryName()+"setUseUnpolarizedParticles";
 cmdUseUnpolarizedParticles = new G4UIcmdWithABool (cmdName, this );
 cmdUseUnpolarizedParticles->SetGuidance( "Set unpolarized particles generation" );
}

void GateJPETSourceMessenger::DeleteCommands()
{
 delete cmdSeedForRandomGenerator;
 delete cmdPromptGammaEnergy;
 delete cmdLinearPolarization;
 delete cmdUseUnpolarizedParticles;
}

void GateJPETSourceMessenger::SetNewValue( G4UIcommand* command, G4String newValue )
{
 if ( command == cmdSeedForRandomGenerator )
  pSource->SetSeedForRandomGenerator( static_cast<unsigned int>( cmdSeedForRandomGenerator->GetNewIntValue( newValue ) ) );
 else if ( command == cmdPromptGammaEnergy )
  pSource->SetPromptGammaEnergy( cmdPromptGammaEnergy->GetNewDoubleValue( newValue ) );
 else if ( command == cmdLinearPolarization )
  pSource->SetLinearPolarizationAngle( cmdLinearPolarization->GetNewDoubleRawValue( newValue ) );
 else if ( command == cmdUseUnpolarizedParticles )
  pSource->SetUnpolarizedParticlesGenerating( cmdUseUnpolarizedParticles->GetNewBoolValue( newValue ) );
 else
  GateVSourceMessenger::SetNewValue( command, newValue );
}


