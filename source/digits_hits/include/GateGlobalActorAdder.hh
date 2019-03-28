#ifndef GateGlobalActorAdder_hh
#define GateGlobalActorAdder_hh

#include "GateGlobalActorHit.hh"
#include <vector>

typedef std::vector< GateGlobalActorHit > GateGlobalActorHits;

class GateGlobalActorAdder
{
 public:
  GateGlobalActorAdder();
  ~GateGlobalActorAdder();

  void processHit( const GateGlobalActorHit& hit );

  GateGlobalActorHits getHits() const;
  
  void setTimeIntervalBetweenHits( const G4double& time );

  void reset();

  bool isReadyToMerge( const GateGlobalActorHit& hit ) const;

  G4int getCurrentProcessedEventID() const;

 private:
  GateGlobalActorHits::reverse_iterator findLastHitInTheSameScintillator( const GateGlobalActorHit& hit );
  bool isCommonTimeInterval( const GateGlobalActorHit& hit_a, const GateGlobalActorHit& hit_b ) const;
  void insertHit( const GateGlobalActorHit& hit );
  void mergeHits( const GateGlobalActorHit& hit_source, GateGlobalActorHit& hit_to_update );

 private:
  GateGlobalActorHits mHits;
  G4double mTimeInterval = 0.0;//ns
  const G4int cNullEvent = -1;
  G4int mCurrentProcessedEvent = cNullEvent;
   
};

#endif
