#include "GatePositroniumDecayInfo.hh"
namespace GatePositroniumDecay {
  GammaType IntToGammaType(const G4int value) {
    switch (value) {
      case 0:
        return GammaType::Unknown;
      case 1:
        return GammaType::SinglePhoton;
      case 2:
        return GammaType::AnnihilationPhoton;
      case 3:
        return GammaType::PromptPhoton;
      default:
        return GammaType::Unknown;
    }
  }
  
  SourceType IntToSourceType(const G4int value) {
    switch (value) {
      case 0:
        return SourceType::Unknown;
      case 1:
        return SourceType::SingleGamaEmitter;
      case 2:
        return SourceType::ParaPositronium;
      case 3:
        return SourceType::OrthoPositronium;
      default:
        return SourceType::Unknown;
    }
  }
  
  DecayType IntToDecayType(const G4int value) {
    switch (value) {
      case 0:
        return DecayType::Unknown;
      case 1:
        return DecayType::StandardChannel;
      case 2:
        return DecayType::DeexciatationChannel;
      default:
        return DecayType::Unknown;
    }
  }
  
  G4int GetMaxPhotonsNumber(const SourceType source_type,const DecayType decay_type) {
    bool unknown_model = source_type == SourceType::Unknown || decay_type == DecayType::Unknown;
    if (unknown_model){
      return 0;
    }
    if (source_type == SourceType::SingleGamaEmitter) {
      return 1;
    }
    if (source_type == SourceType::ParaPositronium) {
      return decay_type == DecayType::DeexciatationChannel ? 3 : 2;
    }
    //OrthoPositronium source
    return decay_type == DecayType::DeexciatationChannel ? 4 : 3;
  }
  
  void GatePositroniumDecayInfo::Fill(const GateCrystalHitsCollection* collection) {
    const G4int n_hits = collection->GetSize();
    for (G4int i = 0; i < n_hits; ++i) {
      const GateCrystalHit* hit = (*collection)[i];
      gammaTypes.emplace(IntToGammaType(hit->GetGammaType()));
      sourceTypes.emplace(IntToSourceType(hit->GetSourceType()));
      decayTypes.emplace(IntToDecayType(hit->GetDecayType()));
    }
  }
  
  bool GatePositroniumDecayInfo::HasGamma(const GammaType gamma_type) const {
    return gammaTypes.find(gamma_type) != gammaTypes.cend();
  }
  
  bool GatePositroniumDecayInfo::HasSource(const SourceType source_type) const {
    return sourceTypes.find(source_type) != sourceTypes.cend();
  }
  
  bool GatePositroniumDecayInfo::HasDecay(const DecayType decay_type) const {
    return decayTypes.find(decay_type) != decayTypes.cend();
  }
  
}