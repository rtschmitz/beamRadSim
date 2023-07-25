/*
 * brTrackingAction.cc
 *
 *  Created on: 22.04.2019
 *      Author: schmitz
 */



#include "brTrackingAction.hh"
#include "brUserTrackInformation.hh"
#include "brDetectorConstruction.hh"
#include "brHistoManager.hh"
#include "brUserEventInformation.hh"
#include "G4ThreeVector.hh"

#include "G4TrackingManager.hh"
#include "G4EventManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"
// Includes Physical Constants and System of Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
brTrackingAction::brTrackingAction(brHistoManager* histo):
	histoManager(histo), verbose(0),
	photonTrackStorage(true),
	gammaTrackStorage(true),
	neutronTrackStorage(true),
	muonTrackStorage(true),
	trackID(-1),parentID(-1),
	particleName(""),
	initialVolumeName(""),
	initialProcessName(""),
	initialPosition(0.,0.,0.),
	initialEnergy(0.),
	initialTime(0.){

}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void brTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  ////Let this be up to the user via vis.mac
  //fpTrackingManager->SetStoreTrajectory(true);
  //
  ////Use custom trajectory class, but not now
  //fpTrackingManager->SetTrajectory(new brTrajectory(aTrack));

  //New track, add the user information object
  //This user track information is only relevant to the photons

  // Check if the track already has track information
  if(aTrack->GetUserInformation()!=0) return;


  brUserEventInformation* eventInformation
  =(brUserEventInformation*)G4EventManager::GetEventManager()->GetUserInformation();

  fpTrackingManager->SetUserTrackInformation(new brUserTrackInformation);

	if (aTrack != NULL) {
		trackID = aTrack->GetTrackID();
	} else {
		trackID = -1;
		G4cerr
				<< "!>  BeginOfTrack: G4Track is NULL!"
				<< G4endl;
	}

	//Get general information about this track
	parentID = aTrack->GetParentID();
	initialVolumeName = aTrack->GetVolume()->GetName();

	//Primary track has no creator process, so ensure that it exists before you try to access it.
	const G4VProcess* myProcess = aTrack->GetCreatorProcess();
	if (myProcess != NULL) {
		initialProcessName = myProcess->GetProcessName();
	} else {
		initialProcessName = "(Primary particle)";
	}

	initialPosition = aTrack->GetVertexPosition();
	initialEnergy = aTrack->GetVertexKineticEnergy();
	initialTime = aTrack->GetGlobalTime();

	particleName = aTrack->GetDefinition()->GetParticleName();

	if( particleName.contains("gamma") //&& (eventInformation->GetGammaLastTrackID() != trackID)
				&& gammaTrackStorage){


			eventInformation->SetGammaLastTrackID(trackID);

			brGammaTrack *gammaTrack = new brGammaTrack();

			gammaTrack->SetTrackID(trackID);
			gammaTrack->SetParentID(parentID);

			//begin of track information
			gammaTrack->SetTimeOfFirstProcess(initialTime/ns);
			gammaTrack->SetInitialEnergy(initialEnergy/MeV);
			gammaTrack->SetFirstPositionX(initialPosition.getX()/m);
			gammaTrack->SetFirstPositionY(initialPosition.getY()/m);
			gammaTrack->SetFirstPositionZ(initialPosition.getZ()/m);

			eventInformation->AddGammaTrack(gammaTrack);
	}

	//
	//Store information about new neutron tracks;
	//
	if( particleName.contains("neutron") //&& (eventInformation->GetNeutronLastTrackID() != trackID)
			&& neutronTrackStorage){
		eventInformation->SetNeutronLastTrackID(trackID);

		brNeutronTrack *neutronTrack = new brNeutronTrack();

		neutronTrack->SetTrackID(trackID);
		neutronTrack->SetParentID(parentID);

		//begin of track information
		neutronTrack->SetTimeOfFirstProcess(initialTime/s);
		neutronTrack->SetInitialEnergy(initialEnergy/MeV);
		neutronTrack->SetFirstPositionX(initialPosition.getX()/m);
		neutronTrack->SetFirstPositionY(initialPosition.getY()/m);
		neutronTrack->SetFirstPositionZ(initialPosition.getZ()/m);
		neutronTrack->SetFirstVolume(initialVolumeName);
		neutronTrack->SetFirstProcessName(initialProcessName);

		eventInformation->AddNeutronTrack(neutronTrack);
	}


	//
	//Store info about muons
	//	
//	if( ((particleName.contains("mu-"))||(particleName.contains("mu+"))) //&& (eventInformation->GetMuonLastTrackID() != trackID)
	if( (particleName.contains("mu")) //&& (eventInformation->GetMuonLastTrackID() != trackID)
				&& muonTrackStorage){
		eventInformation->SetMuonLastTrackID(trackID);

		brMuonTrack *muonTrack = new brMuonTrack();

		muonTrack->SetTrackID(trackID);
		muonTrack->SetParentID(parentID);

		//begin of track information
		muonTrack->SetTimeOfFirstProcess(initialTime/s);
		muonTrack->SetInitialEnergy(initialEnergy/MeV);
		muonTrack->SetFirstPositionX(initialPosition.getX()/m);
		muonTrack->SetFirstPositionY(initialPosition.getY()/m);
		muonTrack->SetFirstPositionZ(initialPosition.getZ()/m);
		muonTrack->SetFirstVolume(initialVolumeName);
		muonTrack->SetFirstProcessName(initialProcessName);

		eventInformation->AddMuonTrack(muonTrack);
	}
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void brTrackingAction::PostUserTrackingAction(const G4Track* aTrack){

	//brTrajectory* trajectory=(brTrajectory*)fpTrackingManager->GimmeTrajectory();

	brUserTrackInformation*
    	trackInformation = (brUserTrackInformation*)aTrack->GetUserInformation();

    brUserEventInformation* eventInformation
    =(brUserEventInformation*)G4EventManager::GetEventManager()->GetUserInformation();


    //
	// Get general information about this track
	// Get the last process of the track via the last stepPoint of the track
    //
	G4StepPoint *finalStepPoint = aTrack->GetStep()->GetPostStepPoint();
	//G4StepPoint *prefinalStepPoint = aTrack->GetStep()->GetPreStepPoint();
	G4String finalVolumeName;
	G4ThreeVector finalPosition = aTrack->GetPosition();
	G4ThreeVector finalDirection = aTrack->GetMomentumDirection();
	G4double finalEnergy = aTrack->GetKineticEnergy();
	G4double totalEnergy = aTrack->GetTotalEnergy();
//	G4double energyDeposit= aTrack->GetEnergyDeposit();
	G4double finalTime = aTrack->GetGlobalTime();
	G4double finalTrackLength = aTrack->GetTrackLength();
	G4int trackID = aTrack->GetTrackID();
	G4int parentID = aTrack->GetParentID();
    G4double muDistanceToTank= 0.;
	if (finalStepPoint != NULL) {
			if (finalStepPoint->GetPhysicalVolume() != NULL) {
				finalVolumeName =
						finalStepPoint->GetPhysicalVolume()->GetName();
			}
		}
    G4String myEndProcessName = "";


        if (finalStepPoint != NULL) {
                        if (finalStepPoint->GetPhysicalVolume() != NULL) {
                                finalVolumeName =
                                                finalStepPoint->GetPhysicalVolume()->GetName();
                        }

                        if (finalStepPoint->GetProcessDefinedStep() != NULL) {
                                                myEndProcessName =
                                                                finalStepPoint->GetProcessDefinedStep()->GetProcessName();
                                        }
                }

	//
	//
	if( particleName.contains("gamma")// && (eventInformation->GetGammaLastTrackID() != trackID)
			&& gammaTrackStorage){


		//end of track information
	    eventInformation->GetGammaTrack(trackID)->SetLastPositionX(finalPosition.getX()/m);
	    eventInformation->GetGammaTrack(trackID)->SetLastPositionY(finalPosition.getY()/m);
	    eventInformation->GetGammaTrack(trackID)->SetLastPositionZ(finalPosition.getZ()/m);
//	    eventInformation->GetGammaTrack(trackID)->SetEnergyDeposit(energyDeposit/MeV);
	    eventInformation->GetGammaTrack(trackID)->SetFinalEnergy(finalEnergy/MeV);
	    eventInformation->GetGammaTrack(trackID)->SetTotalEnergy(totalEnergy/MeV);
	    eventInformation->GetGammaTrack(trackID)->SetTotalTrackLength(finalTrackLength/m);
	    eventInformation->GetGammaTrack(trackID)->SetTimeOfLastProcess(finalTime/ns);


	}
	//
	//Store information about new neutron tracks;
	//
	if( particleName.contains("neutron") //&& (eventInformation->GetNeutronLastTrackID() != trackID)
			&& neutronTrackStorage){

		//end of track information
		eventInformation->GetNeutronTrack(trackID)->SetLastPositionX(finalPosition.getX()/m);
		eventInformation->GetNeutronTrack(trackID)->SetLastPositionY(finalPosition.getY()/m);
		eventInformation->GetNeutronTrack(trackID)->SetLastPositionZ(finalPosition.getZ()/m);
		eventInformation->GetNeutronTrack(trackID)->SetFinalEnergy(finalEnergy/MeV);
		eventInformation->GetNeutronTrack(trackID)->SetTotalEnergy(totalEnergy/MeV);
        eventInformation->GetNeutronTrack(trackID)->SetTotalTrackLength(finalTrackLength/m);
	    eventInformation->GetNeutronTrack(trackID)->SetLastVolume(finalVolumeName);
	    eventInformation->GetNeutronTrack(trackID)->SetLastProcessName(myEndProcessName);
        eventInformation->GetNeutronTrack(trackID)->SetTimeOfLastProcess(finalTime/s);

		//G4cout << "Total energy neutron: " << totalEnergy/MeV << G4endl;
		//G4cout << "Final energy neutron: " << finalEnergy/MeV << G4endl;

	}

	//
	//Store information about muon tracks
	//
//	if( ((particleName.contains("mu-"))||(particleName.contains("mu+"))) //&& (eventInformation->GetNeutronLastTrackID() != trackID)
	if( (particleName.contains("mu")) //&& (eventInformation->GetMuonLastTrackID() != trackID)
			&& muonTrackStorage){

		//end of track information
		eventInformation->GetMuonTrack(trackID)->SetLastPositionX(finalPosition.getX()/m);
		eventInformation->GetMuonTrack(trackID)->SetLastPositionY(finalPosition.getY()/m);
		eventInformation->GetMuonTrack(trackID)->SetLastPositionZ(finalPosition.getZ()/m);
		eventInformation->GetMuonTrack(trackID)->SetFinalEnergy(finalEnergy/MeV);
//		eventInformation->GetMuonTrack(trackID)->SetEnergyDeposit(energyDeposit/MeV);
		eventInformation->GetMuonTrack(trackID)->SetTotalEnergy(totalEnergy/MeV);
		eventInformation->GetMuonTrack(trackID)->SetTotalTrackLength(finalTrackLength/m);
		eventInformation->GetMuonTrack(trackID)->SetLastVolume(finalVolumeName);
	    	eventInformation->GetMuonTrack(trackID)->SetLastProcessName(myEndProcessName);
        	eventInformation->GetMuonTrack(trackID)->SetTimeOfLastProcess(finalTime/s);
		G4double initialEnergy_MeV = eventInformation->GetMuonTrack(trackID)->GetInitialEnergy();
		eventInformation->GetMuonTrack(trackID)->SetEnergyExitScinti(initialEnergy_MeV - finalEnergy/MeV);

		//G4cout << "Total energy neutron: " << totalEnergy/MeV << G4endl;
		//G4cout << "Final energy neutron: " << finalEnergy/MeV << G4endl;

	}



    //========================================================================================================================
     //========================================================================================================================

     //char gNucl[10];
     //strcpy(gNucl, "");
     //std::vector<G4double> gKineticEnergy;
     //gKineticEnergy.clear();

     G4int gMulti = 0;
     G4double gSumEnergy = 0.0;
     G4double gSumGammaEnergy = 0.0;

	//
	//info about optical photons tracks
	//
	if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
			// connect optical photon track info (which hit a PMT) with according event
			// information can only be stored in Post Tracking
			// as it is only known here if the photon hit the PMT
			if(photonTrackStorage
					//&&  (eventInformation->GetPhotonLastTrackID() != trackID)
					){
				eventInformation->SetPhotonLastTrackID(trackID);

				brPhotonTrack *photonTrack = new brPhotonTrack();

				photonTrack->SetTrackID(trackID);
				photonTrack->SetParentID(parentID);
            
				////begin of track information
				if (myEndProcessName == "OpAbsorption") photonTrack->SetAbsorption(true);
				photonTrack->SetTimeOfFirstProcess(initialTime/s);
				photonTrack->SetInitialEnergy(initialEnergy/eV);
				photonTrack->SetFirstPositionX(initialPosition.getX()/m);
				photonTrack->SetFirstPositionY(initialPosition.getY()/m);
				photonTrack->SetFirstPositionZ(initialPosition.getZ()/m);

                if(initialEnergy > 5*eV){
//                		std::cout << "volume: " << initialVolumeName << std::endl;
  //              		std::cout << "process: " << initialProcessName << std::endl;
    //            		std::cout << "final volume: " << finalVolumeName << std::endl;
      //         		std::cout << "final process: " << myEndProcessName << std::endl;
                }
//			std::cout << "tracking finished" << std::endl;
				////end of track information
			    photonTrack->SetLastPositionX(finalPosition.getX()/m);
			    photonTrack->SetLastPositionY(finalPosition.getY()/m);
			    photonTrack->SetLastPositionZ(finalPosition.getZ()/m);
			    photonTrack->SetFinalEnergy(finalEnergy/eV);
			    photonTrack->SetTotalEnergy(totalEnergy/eV);
		        photonTrack->SetTotalTrackLength(finalTrackLength/m);
			    photonTrack->SetTimeOfLastProcess(finalTime/s);
			    photonTrack->SetNbOfReflections(trackInformation->GetInternalReflectionCount());
			    eventInformation->AddPhotonTrack(photonTrack);
			}

			//
            //
			//}
			//Lets choose to draw only the photons that hit a pmt, but not now
			//trajectory->SetDrawTrajectory(true);
		//}
  }

	//else //draw all other trajectories
	//	trajectory->SetDrawTrajectory(true);

  //if(recorder)recorder->Record)Track(aTrack);



}



