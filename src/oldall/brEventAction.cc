/*
 * brEventAction.cc
 *
 *  Created on: 18.09.2012
 *      Author: schmitz
 */

#include "brEventAction.hh"
#include "brPMTHit.hh"
#include "brScintHit.hh"
#include "brUserEventInformation.hh"
#include "brHistoManager.hh"
#include "brROOTEvent.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"


//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
brEventAction::brEventAction(brHistoManager* histo):
  histoManager(histo),saveThreshold(0),pmtCollID(-1),crystalCollID(-1),verbose(0),
   pmtThreshold(1),forcedrawphotons(false),forcenophotons(false), pmtnb(0),//crystalnb(0),
   storePMTHit(false),storeCrystalHit(true)
{

}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
brEventAction::~brEventAction(){


}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void brEventAction::BeginOfEventAction(const G4Event* anEvent){

  //New event, add the user information object
  G4EventManager::
    GetEventManager()->SetUserInformation(new brUserEventInformation);

  // Find the sensitive detectors by name
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  if( pmtCollID < 0 ){

    pmtCollID=SDman->GetCollectionID("pmtHitCollection");

  }

  if( crystalCollID < 0 ){

      crystalCollID=SDman->GetCollectionID("scintCollection");
  }
  
G4int eventID = -1;

	//Get event ID
	if (anEvent != NULL) {
		eventID = anEvent->GetEventID();
	} else {
		eventID = -1;
		G4cerr << "!> BeginOfEvent run is NULL!" << G4endl;
	}

	// get User event information and reset it as it is the beginning of the event

	brUserEventInformation* eventInformation
      =(brUserEventInformation*)anEvent->GetUserInformation();

//	eventInformation->Reset();

	eventInformation->SetEventID(eventID);
	eventInformation->SetPhotonLastTrackID(-1);
	eventInformation->SetGammaLastTrackID(-1);
	eventInformation->SetNeutronLastTrackID(-1);
	eventInformation->SetMuonLastTrackID(-1);

	if (verbose >= 1) {
		G4cout << ">> Enter event #" << eventID << G4endl;
	}

  //if(recorder)recorder->RecordBeginOfEvent(anEvent);
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void brEventAction::EndOfEventAction(const G4Event* anEvent){
  //G4cout << "End of Event Action" << G4endl;
  brUserEventInformation* eventInformation
    =(brUserEventInformation*)anEvent->GetUserInformation();

  //=======================================================================================
  //Hit collections
  //=======================================================================================

  brPMTHitsCollection* PHC = 0; //PMT hit collection
  brScintHitsCollection* THC = 0; // Scintillator hit collection
  G4HCofThisEvent* HCE = anEvent->GetHCofThisEvent();

  //
  //Get the hit collections
  //
  if(HCE){

    if(pmtCollID>=0) PHC = (brPMTHitsCollection*)(HCE->GetHC(pmtCollID));
    if(crystalCollID>=0) THC = (brScintHitsCollection*)(HCE->GetHC(crystalCollID));


  }

  //=======================================================================================
  //Crystal hit collection
  //=======================================================================================


  if(THC){
	  //
	  //add crystal hit vector to the eventInformation
	  //
	  if (storeCrystalHit){
		  brScintHitVector myScintHitsVec = *(THC->GetVector());

		  for (unsigned int j = 0; j < myScintHitsVec.size(); j++) {
		  				eventInformation->AddScintHit(myScintHitsVec[j]);
		  }
	  }

  }

  //=======================================================================================
  // PMT hit collection
  //=======================================================================================
  if(PHC){
	  //
	  //add PMT hit vector to the eventInformation
	  //
	  if (storePMTHit){
		  brPMTHitVector myPMTHitsVec = *(PHC->GetVector());

		  for (unsigned int j = 0; j < myPMTHitsVec.size(); j++) {
	  						eventInformation->AddPMTHit(myPMTHitsVec[j]);
		  }
	  }


    G4int pmts=PHC->entries();

    //
    //Gather info from all PMTs
    //
    for(G4int i=0;i<pmts;i++){

      eventInformation->IncPECountPMT((*PHC)[i]->GetPhotonCount());

      //pmtnb = (*PHC)[i]->GetPMTNumber();

      if((*PHC)[i]->GetPhotonCount() >= pmtThreshold){
    	  eventInformation->IncPMTSAboveThreshold();
      }
      else{//wasnt above the threshold, turn it back off, but not now
    	  //(*PHC)[i]->SetDrawit(false);
      }
    }


    //PHC->DrawAllHits(); not now
  }



  eventInformation->Finalize();


  if(verbose>0){ // TODO put into Print()
	//
    //End of event output. later to be controlled by a verbose level
	//
	G4cout << "\tEvent ID: "
	  	   << anEvent->GetEventID() << G4endl;
    G4cout << "\tNumber of PE in PMTs in this event : "
	   << eventInformation->GetPECountPMT() << G4endl;

    G4cout << "\tNumber of PMTs above threshold("<<pmtThreshold<<") : "
	   << eventInformation->GetPMTSAboveThreshold() << G4endl;
    G4cout << "\tNumber of photons produced  in this event : "
	   << eventInformation->GetPhotonCount() << G4endl;

    G4cout << "\tNumber of photons produced by scintillation in this event : "
	   << eventInformation->GetPhotonCount_Scint() << G4endl;
    G4cout << "\tNumber of photons produced by cherenkov in this event : "
	   << eventInformation->GetPhotonCount_Cheren() << G4endl;
    G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
	   << eventInformation->GetAbsorptionCount() << G4endl;
    G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
	   << "this event : " << eventInformation->GetBoundaryAbsorptionCount()
	   << G4endl;
    G4cout << "\tUnacounted for photons in this event : "
	   << (
	       eventInformation->GetPhotonCount_Scint()+
	       eventInformation->GetPhotonCount_Cheren()-
	       eventInformation->GetAbsorptionCount() -
	       eventInformation->GetPECountPMT() -
	       eventInformation->GetBoundaryAbsorptionCount())
	   << G4endl;


    G4cout << "\tNumber of neutrons in this event: "
    		<< eventInformation->GetNeutronTracks()->size()
    		<< G4endl;
    if (verbose > 1){
    		for (G4int n  = 0; n < eventInformation->GetNeutronTracks()->size(); n++){
    G4cout << "\tNeutron creation process: "
       		<< eventInformation->GetNeutronTracks()->at(n)->GetFirstProcessName()
       		<< G4endl;
    		}
    }
  }



  //
  //If we have set the flag to save 'special' events, save here
  //
  if(saveThreshold&&eventInformation->GetPhotonCount_Scint() <= saveThreshold)
    G4RunManager::GetRunManager()->rndmSaveThisEvent();

  //if(recorder)recorder->RecordEndOfEvent(anEvent);
  //




  //
  //fill ntuple via HistoManager for chosen characteristics of the event
  //

  brROOTEvent* rootEvent = eventInformation->ConvertToROOTEvent();

  rootEvent->Finalize();

  histoManager->FillEventNtuple(rootEvent);

//  rootEvent->Reset();
  //eventInformation->Reset();
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void brEventAction::SetSaveThreshold(G4int save){
/*Sets the save threshold for the random number seed. If the number of photons
generated in an event is lower than this, then save the seed for this event
in a file called run###evt###.rndm
*/
  saveThreshold=save;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");
  //  G4UImanager::GetUIpointer()->ApplyCommand("/random/setSavingFlag 1");
}




