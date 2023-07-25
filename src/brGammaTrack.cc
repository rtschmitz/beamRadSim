/*
 * brGammaTrack.cc
 *
 * Gamma track class. This class allows to store all necessary information about
 * muon tracks such as ID, track length, ... .
 * It inherits from TObject and can be directly stored into a TTree.
 * Track informations are assigned in the brTrackingAction.
 *
 * Created on: 22.04.2019
 * Author: schmitz
 */
#include "brGammaTrack.hh"
#include "TObject.h"

ClassImp(brGammaTrack)

//==============================================================================

brGammaTrack::brGammaTrack() :
			trackID(-1),
			initialTime_ns(0.), finalTime_ns(0.),
			initialEnergy_MeV(0.),finalEnergy_MeV(0.),
			totalEnergy_MeV(0.), energyDeposit_MeV(0.),
			parentID(-1),
			initialPositionX_m(0.),finalPositionX_m(0.),
			initialPositionY_m(0.),finalPositionY_m(0.),
			initialPositionZ_m(0.),finalPositionZ_m(0.),
			totalTrackLength_m(0.0)


			{

}


//==============================================================================
brGammaTrack::~brGammaTrack() {
}
const brGammaTrack& brGammaTrack::operator=(const brGammaTrack &right){

	trackID 		      = right.trackID 		     ;
	initialTime_ns        = right.initialTime_ns       ;
	finalTime_ns          = right.finalTime_ns         ;
	initialEnergy_MeV     = right.initialEnergy_MeV    ;
	finalEnergy_MeV       = right.finalEnergy_MeV      ;
	energyDeposit_MeV     = right.energyDeposit_MeV      ;
	totalEnergy_MeV       = right.totalEnergy_MeV       ;
	parentID              = right.parentID             ;
	initialPositionX_m    = right.initialPositionX_m   ;
	finalPositionX_m      = right.finalPositionX_m     ;
	initialPositionY_m    = right.initialPositionY_m   ;
	finalPositionY_m      = right.finalPositionY_m     ;
	initialPositionZ_m    = right.initialPositionZ_m   ;
	finalPositionZ_m      = right.finalPositionZ_m     ;
	totalTrackLength_m    = right.totalTrackLength_m   ;
	gammaOutScint         = right.gammaOutScint        ;
	return *this;
}

Int_t brGammaTrack::operator==(const brGammaTrack& right) const
{
  return (this==&right) ? 1 : 0;
}

//==============================================================================



bool brGammaTrack::compareHits(brGammaTrack* const &a, brGammaTrack* const &b){
	Double_t aTime = a->GetTimeOfLastProcess();
	Double_t bTime = b->GetTimeOfLastProcess();
	return (aTime < bTime);
}





