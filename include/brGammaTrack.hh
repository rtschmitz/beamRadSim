/*
 * brGammaTrack.hh
 *
 * Gamma track class. This class allows to store all necessary information about
 * muon tracks such as ID, track length, ... .
 * It inherits from TObject and can be directly stored into a TTree.
 * Track informations are assigned in the brTrackingAction.
 *
 * Created on: 22.04.2019
 * Author: Schmitz
 */

#ifndef BRGAMMATRACK_HH_
#define BRGAMMATRACK_HH_

#include <iostream>
#include <vector>
#include "TObject.h"
#include "TString.h"
#include "TVector3.h"
//==============================================================================
class brGammaTrack: public TObject {
public:
	brGammaTrack();
	~brGammaTrack();
	 const brGammaTrack& operator=(const brGammaTrack &right);
     Int_t operator==(const brGammaTrack&) const;


	void SetTrackID(Int_t tID){trackID = tID; }
	void SetTimeOfFirstProcess(Double_t timeOfFirstProcess){ initialTime_ns = timeOfFirstProcess; }
	void SetTimeOfLastProcess(Double_t timeOfLastProcess){ finalTime_ns = timeOfLastProcess; }
	void SetInitialEnergy(Double_t firstEnergy){ initialEnergy_MeV = firstEnergy; }
	void SetFinalEnergy(Double_t lastEnergy){ finalEnergy_MeV = lastEnergy; }
	void SetTotalEnergy(Double_t totalEnergy){totalEnergy_MeV = totalEnergy; }
	void SetEnergyDeposit(Double_t depositEnergy){energyDeposit_MeV = depositEnergy; }
	void SetParentID(Int_t pID){ parentID = pID; }
	void SetFirstPositionX(Double_t firstPositionX){ initialPositionX_m = firstPositionX; }
	void SetLastPositionX(Double_t lastPositionX) { finalPositionX_m = lastPositionX; }
	void SetFirstPositionY(Double_t firstPositionY){ initialPositionY_m = firstPositionY; }
	void SetLastPositionY(Double_t lastPositionY) { finalPositionY_m = lastPositionY; }
	void SetFirstPositionZ(Double_t firstPositionZ){ initialPositionZ_m = firstPositionZ; }
	void SetLastPositionZ(Double_t lastPositionZ){ finalPositionZ_m = lastPositionZ; }
	void SetTotalTrackLength(Double_t totalMuTrackLength){ totalTrackLength_m = totalMuTrackLength; }


	Int_t GetTrackID()const{ return trackID; }
	Double_t GetTimeOfFirstProcess()const{ return initialTime_ns; }
	Double_t GetTimeOfLastProcess()const{ return finalTime_ns; }
	Double_t GetInitialEnergy()const{ return initialEnergy_MeV; }
	Double_t GetFinalEnergy()const{ return finalEnergy_MeV; }
	Double_t GetEnergyDeposit()const{ return energyDeposit_MeV; }
	Int_t 	 GetParentID()const{ return parentID; }
	Double_t GetFirstPositionX()const{ return initialPositionX_m; }
	Double_t GetLastPositionX()const{ return finalPositionX_m; }
	Double_t GetFirstPositionY()const{ return initialPositionY_m; };
	Double_t GetLastPositionY()const{ return finalPositionY_m; }
	Double_t GetFirstPositionZ()const{ return initialPositionZ_m; };
	Double_t GetLastPositionZ()const{ return finalPositionZ_m; }
	Double_t GetTotalTrackLength()const{ return totalTrackLength_m; }
	Double_t GetTotalEnergy() const{return totalEnergy_MeV; }
	//Gamma track information

	void SetGammaOutScintillator(bool outScint) {gammaOutScint = outScint;}
	bool GetGammaOutScintillator()const{return gammaOutScint;}

	// order tracks accordong to timing information
	static bool compareHits(brGammaTrack* const &a, brGammaTrack* const &b);
	//friend std::ostream& operator<<(std::ostream &o,
	//		brGammaTrack const &track);

	private:

	Int_t trackID;
	Double_t initialTime_ns       ; //Global time when the first process occurs
 	Double_t finalTime_ns         ; //Global time when the last process occurs
	Double_t initialEnergy_MeV     ; //The kinetic energy at the begin of the track
	Double_t finalEnergy_MeV       ; //The kinetic energy at the end of the track
	Double_t energyDeposit_MeV       ; //The kinetic energy at the end of the track
	Double_t totalEnergy_MeV;       //Energy loss of the track
	Int_t parentID             ; //The track ID of the parent track. If the track is a primary then it's zero.
	Double_t initialPositionX_m  ; //The position of the vertex of the track
	Double_t finalPositionX_m    ; //The position of the end of the track
	Double_t initialPositionY_m  ; //The position of the vertex of the track
	Double_t finalPositionY_m    ; //The position of the end of the track
	Double_t initialPositionZ_m  ; //The position of the vertex of the track
	Double_t finalPositionZ_m    ; //The position of the end of the track
	Double_t totalTrackLength_m  ; //The total track length of the track
	bool gammaOutScint;		//The gamma exited the scintillator to the world without detection == true

	ClassDef(brGammaTrack,7);


};

typedef std::vector<brGammaTrack*> brGammaTrackVector;

#endif /* BRGAMMATRACK_HH_ */
