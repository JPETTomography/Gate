#ifndef POSITRONIUM_DECAY_INFO_HH
#define POSITRONIUM_DECAY_INFO_HH
#include "GateCrystalHit.hh"
#include <set>

namespace GatePositroniumDecay {
  enum class GammaType {Unknown = 0,SinglePhoton = 1,AnnihilationPhoton = 2,PromptPhoton = 3};
  enum class SourceType {Unknown = 0, SingleGamaEmitter = 1, ParaPositronium = 2, OrthoPositronium = 3 };
  enum class DecayType {Unknown = 0, StandardChannel = 1, DeexciatationChannel = 2};
  
  GammaType IntToGammaType(const G4int value);
  SourceType IntToSourceType(const G4int value);
  DecayType IntToDecayType(const G4int value);
  G4int GetMaxPhotonsNumber(const SourceType source_type,const DecayType decay_type);
  
  class GatePositroniumDecayInfo {
    public:
      void Fill(const GateCrystalHitsCollection* collection);
      bool HasGamma(const GammaType gamma_type) const;
      bool HasSource(const SourceType source_type) const;
      bool HasDecay(const DecayType decay_type) const;
    private:
      std::set<GammaType> gammaTypes;
      std::set<SourceType> sourceTypes;
      std::set<DecayType> decayTypes;
  };
}
#endif