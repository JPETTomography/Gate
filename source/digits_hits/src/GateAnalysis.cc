/*----------------------
  Copyright (C): OpenGATE Collaboration

  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/

#include "globals.hh"
#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4VHitsCollection.hh"
#include "G4HCofThisEvent.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VProcess.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4ParticleDefinition.hh"
#include "G4Positron.hh"
#include "G4GenericIon.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

#include "GatePrimaryGeneratorAction.hh"
#include "GateSourceMgr.hh"
#include "GateCrystalHit.hh"
#include "GatePhantomHit.hh"
#include "GateAnalysis.hh"
#include "GateAnalysisMessenger.hh"
#include "GateTrajectoryNavigator.hh"
#include "GateOutputMgr.hh"
#include "GateVVolume.hh"
#include "GateActions.hh"
#include "GateToRoot.hh"
//--------------------------------------------------------------------------------------------------
GateAnalysis::GateAnalysis(const G4String& name, GateOutputMgr* outputMgr,DigiMode digiMode)
  : GateVOutputModule(name,outputMgr,digiMode)
{
  m_isEnabled = true; // This module is essential for the hit processing, so let it enabled !!!
  m_analysisMessenger = new GateAnalysisMessenger(this);
  m_trajectoryNavigator = new GateTrajectoryNavigator();
  SetVerboseLevel(0);
}
//--------------------------------------------------------------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...

//--------------------------------------------------------------------------------------------------
GateAnalysis::~GateAnalysis()
{
  delete m_analysisMessenger;
  //modifs seb 3/3/2009
  delete m_trajectoryNavigator;
  if (nVerboseLevel > 0)
    G4cout << "GateAnalysis deleting...m_analysisMessenger - m_trajectoryNavigator\n";
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
const G4String& GateAnalysis::GiveNameOfFile()
{
  m_noFileName = "  "; // 2 spaces for output module with no fileName
  return m_noFileName;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void GateAnalysis::RecordBeginOfAcquisition()
{
  if (nVerboseLevel > 2)
    G4cout << "GateAnalysis::RecordBeginOfAcquisition\n";
}
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
void GateAnalysis::RecordEndOfAcquisition()
{
  if (nVerboseLevel > 2)
    G4cout << "GateAnalysis::RecordEndOfAcquisition\n";
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void GateAnalysis::RecordBeginOfRun(const G4Run * )
{
  if (nVerboseLevel > 2)
    G4cout << "GateAnalysis::RecordBeginOfRun\n";
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void GateAnalysis::RecordEndOfRun(const G4Run * )
{
  if (nVerboseLevel > 2)
    G4cout << "GateAnalysis::RecordEndOfRun\n";
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void GateAnalysis::RecordBeginOfEvent(const G4Event* )
{
  if (nVerboseLevel > 2)
    G4cout << "GateAnalysis::RecordBeginOfEvent\n";
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void GateAnalysis::RecordEndOfEvent(const G4Event* event)
{
  if (nVerboseLevel > 2)
    G4cout << "GateAnalysis::RecordEndOfEvent" << G4endl;

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();

  if (trajectoryContainer) {
    m_trajectoryNavigator->SetTrajectoryContainer(trajectoryContainer);
  }

  G4int eventID = event->GetEventID();
  G4int runID   = GateRunManager::GetRunManager()->GetCurrentRun()->GetRunID();

  if (!trajectoryContainer) {
    if (nVerboseLevel > 0) {
      G4cout << "GateAnalysis::RecordEndOfEvent : WARNING : G4TrajectoryContainer not found" << G4endl;
    }
  } else {
    GateCrystalHitsCollection* CHC = GetOutputMgr()->GetCrystalHitCollection();
    G4int NbHits = 0;
    G4int NpHits = 0;

    if (CHC) {
      NbHits = CHC->entries();
      std::vector<PhotonScatterings> photon_scatterings(3);
      G4int rootID     = 0;
      G4int primaryID  = 0;
      G4int septalNb = 0; // HDS : septal penetration

      m_trajectoryNavigator->FindPositronTrackID();
	    //search the two gammas
      std::vector<G4int> photonIDVec = m_trajectoryNavigator->FindAnnihilationGammasTrackID();
      if (photonIDVec.size() == 0) {
        // no gamma coming from a positron or an ion, or shooted as primary
        if (nVerboseLevel > 0) {
          G4cout << "GateAnalysis::RecordEndOfEvent : WARNING : photonIDs not found" << G4endl;
        }
      } else {
        //  This warning is somewhat irrelevant with 124I
        if (nVerboseLevel > 0 && photonIDVec.size() > 2) {
          G4cout << "GateAnalysis::RecordEndOfEvent : WARNING : photonID vector size > 2" << G4endl;
        }
        photon_scatterings[0].photonID = photonIDVec[0];
        photon_scatterings[1].photonID = (photonIDVec.size() >= 2) ? photonIDVec[1] : 0;
        photon_scatterings[2].photonID = (photonIDVec.size() >= 3) ? photonIDVec[2] : 0;
      }

      if (nVerboseLevel > 0) {
        if (photon_scatterings[0].photonID == 0) {
          G4cout << "GateAnalysis::RecordEndOfEvent : WARNING : photon1ID == 0" << G4endl;
        }
        if (photon_scatterings[1].photonID == 0) {
          G4cout << "GateAnalysis::RecordEndOfEvent : WARNING : photon2ID == 0" << G4endl;
        }
        if (photon_scatterings[2].photonID == 0) {
          G4cout << "GateAnalysis::RecordEndOfEvent : WARNING : photon3ID == 0" << G4endl;
        }
        if (nVerboseLevel > 1) {
          G4cout << "GateAnalysis::RecordEndOfEvent : photon1ID : " << photon_scatterings[0].photonID;
          G4cout << "     photon2ID : " << photon_scatterings[1].photonID << G4endl;
        }
      }

      // analysis of the phantom hits to count the comptons, etc.

      GatePhantomHitsCollection* PHC = GetOutputMgr()->GetPhantomHitCollection();
      NpHits = PHC->entries();
      G4String theComptonVolumeName("NULL");
      G4String theRayleighVolumeName("NULL");

      for (G4int iPHit=0;iPHit<NpHits;iPHit++) {
        // HDS : septal penetration record
        if ( m_recordSeptalFlag ) {
          if ((*PHC)[iPHit]->GetPhysVolName() == m_septalPhysVolumeName) {
            ++septalNb;
          }
        }
        G4int phantomTrackID = (*PHC)[iPHit]->GetTrackID();
        G4String processName = (*PHC)[iPHit]->GetProcess();
        G4int PDGcode = (*PHC)[iPHit]->GetPDGEncoding();
        G4ThreeVector hitPos = (*PHC)[iPHit]->GetPos();

        if (nVerboseLevel > 2) {
          G4cout << "GateAnalysis::RecordEndOfEvent : GatePhantomHitsCollection : trackID : " << std::setw(5) << phantomTrackID;
          G4cout << "    PDG code : " << std::setw(5) << PDGcode << "  processName : <" << processName << G4endl;
        }
        
        if ((phantomTrackID == photon_scatterings[0].photonID)||(phantomTrackID == photon_scatterings[1].photonID)) {
          //Modif by DS and LS on Oct 4, 2002: we need to be able to recognise both 'compt'
          //and 'LowEnCompt", hence the find on 'ompt' modif. by CJG to separate Compton and Rayleigh photons
          const bool isComptonInPhantom = processName.find("ompt") != G4String::npos;
          const bool isRayleighInPhantom = processName.find("Rayl") != G4String::npos;
          if (isComptonInPhantom || isRayleighInPhantom) {
            G4Navigator *gNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
            G4ThreeVector zero_vector(0.,0.,0.);
            G4String volume_name = gNavigator->LocateGlobalPointAndSetup(hitPos,&zero_vector,false)->GetName();
            theComptonVolumeName = isComptonInPhantom ? volume_name : theComptonVolumeName;
            theRayleighVolumeName = isRayleighInPhantom ? volume_name : theRayleighVolumeName;
          }
          auto found_photon_scatterings = find_if(photon_scatterings.begin(),photon_scatterings.end(), [&phantomTrackID](PhotonScatterings& ps) { return ps.photonID == phantomTrackID;});
          if (found_photon_scatterings != photon_scatterings.end()) {
            if (isComptonInPhantom) {
              found_photon_scatterings->nPhantomCompton += 1;
              found_photon_scatterings->theComptonVolumeName = theComptonVolumeName;
            } else {
              found_photon_scatterings->nPhantomRayleigh += 1;
              found_photon_scatterings->theRayleighVolumeName = theComptonVolumeName;
            }
          }
        }
      } // end loop NpHits

      TrackingMode theMode =( (GateSteppingAction *)(GateRunManager::GetRunManager()->GetUserSteppingAction() ) )->GetMode();

      if (theMode == TrackingMode::kTracker) {
        // in tracker mode we store the infos about the number of compton and rayleigh
        GateToRoot* gateToRoot = (GateToRoot*) (GateOutputMgr::GetInstance()->GetModule("root"));
        ComptonRayleighData aCRData;
        aCRData.photon1_phantom_Rayleigh = photon_scatterings[0].nPhantomRayleigh;
        aCRData.photon2_phantom_Rayleigh = photon_scatterings[1].nPhantomRayleigh;
        aCRData.photon3_phantom_Rayleigh = photon_scatterings[2].nPhantomRayleigh;
        aCRData.photon1_phantom_compton  = photon_scatterings[0].nPhantomCompton;
        aCRData.photon2_phantom_compton  = photon_scatterings[1].nPhantomCompton;
        aCRData.photon3_phantom_compton  = photon_scatterings[2].nPhantomCompton;
        strcpy(aCRData.theComptonVolumeName1 , photon_scatterings[0].theComptonVolumeName.c_str() );
        strcpy(aCRData.theComptonVolumeName2 , photon_scatterings[1].theComptonVolumeName.c_str() );
        strcpy(aCRData.theComptonVolumeName3 , photon_scatterings[2].theComptonVolumeName.c_str() );
        strcpy(aCRData.theRayleighVolumeName1 , photon_scatterings[0].theRayleighVolumeName.c_str() );
        strcpy(aCRData.theRayleighVolumeName2 , photon_scatterings[1].theRayleighVolumeName.c_str() );
        strcpy(aCRData.theRayleighVolumeName3 , photon_scatterings[2].theRayleighVolumeName.c_str() );
        gateToRoot->RecordPHData( aCRData );
      } else if ( theMode == TrackingMode::kDetector ) {
        // in tracker mode we store the infos about the number of compton and rayleigh - we are in detector mode
        GateToRoot* gateToRoot = (GateToRoot*) (GateOutputMgr::GetInstance()->GetModule("root"));
        ComptonRayleighData aCRData;
        gateToRoot->GetPHData( aCRData);
        photon_scatterings[0].nPhantomRayleigh += aCRData.photon1_phantom_Rayleigh;
        photon_scatterings[1].nPhantomRayleigh += aCRData.photon2_phantom_Rayleigh;
        photon_scatterings[2].nPhantomRayleigh += aCRData.photon3_phantom_Rayleigh;
        photon_scatterings[0].nPhantomCompton += aCRData.photon1_phantom_compton;
        photon_scatterings[1].nPhantomCompton += aCRData.photon2_phantom_compton;
        photon_scatterings[2].nPhantomCompton += aCRData.photon3_phantom_compton;

        photon_scatterings[0].theComptonVolumeName = aCRData.theComptonVolumeName1;
        photon_scatterings[1].theComptonVolumeName = aCRData.theComptonVolumeName2;
        photon_scatterings[2].theComptonVolumeName = aCRData.theComptonVolumeName3;
        photon_scatterings[0].theRayleighVolumeName = aCRData.theRayleighVolumeName1;
        photon_scatterings[1].theRayleighVolumeName = aCRData.theRayleighVolumeName2;
        photon_scatterings[2].theRayleighVolumeName = aCRData.theRayleighVolumeName3;
      }

      // Source info
      // DS : if gate source is not used (with /gate/EnableGeneralParticleSource) there are no GateSource, so skip
      if ((GateSourceMgr::GetInstance())->GetSourcesForThisEvent().size() == 0) {
        return;
      }

      G4int sourceID = (((GateSourceMgr::GetInstance())->GetSourcesForThisEvent())[0])->GetSourceID();
      G4ThreeVector sourceVertex = m_trajectoryNavigator->FindSourcePosition();

      // Hits loop
      for (G4int iHit=0;iHit<NbHits;iHit++) {
        G4int    crystalTrackID = (*CHC)[iHit]->GetTrackID();
        G4String processName = (*CHC)[iHit]->GetProcess();
        auto found_photon_scatterings_crystal = find_if(photon_scatterings.begin(),photon_scatterings.end(), [&crystalTrackID](PhotonScatterings& ps) { return ps.photonID == crystalTrackID;});
        if (found_photon_scatterings_crystal != photon_scatterings.end()) {
          if (processName.find("ompt") != G4String::npos) {
            // Counting Compton in the Crystal
            found_photon_scatterings_crystal->nCrystalCompton += 1;
          } else if (processName.find("Rayl") != G4String::npos) {
            // Counting Rayleigh scatter in crystal
            found_photon_scatterings_crystal->nCrystalRayleigh += 1;
          }
        }

        G4int PDGEncoding  = (*CHC)[iHit]->GetPDGEncoding();
        if (nVerboseLevel > 2) {
          G4cout << "GateAnalysis::RecordEndOfEvent : CrystalHitsCollection: processName : <" << processName << G4endl;
          G4cout << ">    Particls PDG code : " << PDGEncoding << G4endl;
        }
        if ((*CHC)[iHit]->GoodForAnalysis()) {
          // fill in values with the branch with C struct
          G4int trackID  = (*CHC)[iHit]->GetTrackID();
          G4int photonID = 0;
          G4int nPhantomCompton = 0;
          G4int nCrystalCompton = 0;
          G4int nPhantomRayleigh = 0;
          G4int nCrystalRayleigh = 0;

          if (photon_scatterings[0].photonID != 0) { 
            // this means that at least 1 photon has been found, requiring 2 is wrong for SPECT
            // search the gamma from which this hit comes --> photonID
            photonID = m_trajectoryNavigator->FindPhotonID(trackID);
            if (nVerboseLevel > 2 && photonID == rootID) {
              G4cout << "GateAnalysis::RecordEndOfEvent : trackID: " << trackID << " photonID = " << rootID << G4endl;
            }
          }
          
          if (photonID > 0 && photonID < 4) {
            const G4int index = photonID - 1;
            nPhantomCompton = photon_scatterings[index].nPhantomCompton;
            nPhantomRayleigh = photon_scatterings[index].nPhantomRayleigh;
            theComptonVolumeName = photon_scatterings[index].theComptonVolumeName;
            theRayleighVolumeName = photon_scatterings[index].theRayleighVolumeName;
            nCrystalCompton = photon_scatterings[index].nCrystalCompton;
            nCrystalRayleigh = photon_scatterings[index].nCrystalRayleigh;
          }

          // search the primary that originated the track
          primaryID = m_trajectoryNavigator->FindPrimaryID(trackID);
          (*CHC)[iHit]->SetSourceID(sourceID);
          (*CHC)[iHit]->SetSourcePosition(sourceVertex);
          (*CHC)[iHit]->SetNPhantomCompton(nPhantomCompton);
          (*CHC)[iHit]->SetNPhantomRayleigh(nPhantomRayleigh);
          (*CHC)[iHit]->SetComptonVolumeName(theComptonVolumeName);
          (*CHC)[iHit]->SetRayleighVolumeName(theRayleighVolumeName);
          (*CHC)[iHit]->SetPhotonID(photonID);
          (*CHC)[iHit]->SetPrimaryID(primaryID);
          (*CHC)[iHit]->SetEventID(eventID);
          (*CHC)[iHit]->SetRunID(runID);
          (*CHC)[iHit]->SetNCrystalCompton(nCrystalCompton);
          (*CHC)[iHit]->SetNCrystalRayleigh(nCrystalRayleigh);
          (*CHC)[iHit]->SetNSeptal(septalNb); // HDS : septal penetration
        }
      }
    } // end if (CHC)
  } // end if (!trajectoryContainer)
} // end function
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void GateAnalysis::RecordStepWithVolume(const GateVVolume *, const G4Step* )
{
  if (nVerboseLevel > 2)
    G4cout << "GateAnalysis::RecordStep\n";
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void GateAnalysis::SetVerboseLevel(G4int val) {
  nVerboseLevel = val;
  if (m_trajectoryNavigator) m_trajectoryNavigator->SetVerboseLevel(val);
}
//--------------------------------------------------------------------------------------------------
