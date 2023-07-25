/*
 * brMuonTrack.hh
 *
 *  Created on: 7.19.2017
 *      Author: Ryan Schmitz
 */

#ifndef BRMUONTRACK_HH_
#define BRMUONTRACK_HH_

#include <iostream>
#include <vector>
#include "TObject.h"
#include "TString.h"
#include "TVector3.h"
//==============================================================================
class brMuonTrack: public TObject {
public:
	brMuonTrack();
	~brMuonTrack();
	 const brMuonTrack& operator=(const brMuonTrack &right);
     Int_t operator==(const brMuonTrack&) const;


	void SetTrackID(Int_t tID){trackID = tID; }
	void SetFirstProcessName(std::string firstProcessName){ initialProcessName = firstProcessName; }
	void SetLastProcessName(TString lastProcessName){ finalProcessName = lastProcessName; }
	void SetFirstVolume(TString firstVolume){ initialVolumeName = firstVolume; }
	void SetLastVolume(TString lastVolume){ finalVolumeName = lastVolume; }
	void SetTimeOfFirstProcess(Double_t timeOfFirstProcess){ initialTime_s = timeOfFirstProcess; }
	void SetTimeOfLastProcess(Double_t timeOfLastProcess){ finalTime_s = timeOfLastProcess; }
	void SetInitialEnergy(Double_t firstEnergy){ initialEnergy_MeV = firstEnergy; }
	void SetFinalEnergy(Double_t lastEnergy){ finalEnergy_MeV = lastEnergy; }
	void SetEnergyDeposit(Double_t energyDeposit){ energyDeposit_MeV = energyDeposit; }
	void SetTotalEnergy(Double_t totalEnergy){totalEnergy_MeV = totalEnergy; }
	void SetParentID(Int_t pID){ parentID = pID; }
	void SetFirstPositionX(Double_t firstPositionX){ initialPositionX_m = firstPositionX; }
	void SetLastPositionX(Double_t lastPositionX) { finalPositionX_m = lastPositionX; }
	void SetFirstPositionY(Double_t firstPositionY){ initialPositionY_m = firstPositionY; }
	void SetLastPositionY(Double_t lastPositionY) { finalPositionY_m = lastPositionY; }
	void SetFirstPositionZ(Double_t firstPositionZ){ initialPositionZ_m = firstPositionZ; }
	void SetLastPositionZ(Double_t lastPositionZ){ finalPositionZ_m = lastPositionZ; }
	void SetTotalTrackLength(Double_t totalTrackLength){ totalTrackLength_m = totalTrackLength; }

	Int_t GetTrackID()const{ return trackID; }
	TString GetFirstProcessName()const{ return initialProcessName; }
	TString GetLastProcessName()const{ return finalProcessName;}
	TString GetFirstVolume()const{ return initialVolumeName;}
	TString GetLastVolume()const{ return finalVolumeName; }
	Double_t GetTimeOfFirstProcess()const{ return initialTime_s; }
	Double_t GetTimeOfLastProcess()const{ return finalTime_s; }
	Double_t GetInitialEnergy()const{ return initialEnergy_MeV; }
	Double_t GetEnergyDeposit()const{ return energyDeposit_MeV; }
	Double_t GetFinalEnergy()const{ return finalEnergy_MeV; }
	Double_t GetTotalEnergy() const{return totalEnergy_MeV; }

	Int_t 	 GetParentID()const{ return parentID; }
	Double_t GetFirstPositionX()const{ return initialPositionX_m; }
	Double_t GetLastPositionX()const{ return finalPositionX_m; }
	Double_t GetFirstPositionY()const{ return initialPositionY_m; };
	Double_t GetLastPositionY()const{ return finalPositionY_m; }
	Double_t GetFirstPositionZ()const{ return initialPositionZ_m; };
	Double_t GetLastPositionZ()const{ return finalPositionZ_m; }
	Double_t GetTotalTrackLength()const{ return totalTrackLength_m; }

	//Muon track information

	void SetNbOfElastics(Int_t elastics){nbOfElastics = elastics; }
	void SetNbOfInteractions(Int_t interactions){nbOfInteractions = interactions; }
	void AddNbOfElastics(Int_t elastics){nbOfElastics += elastics; }
	void AddNbOfInteractions(Int_t interactions){nbOfInteractions += interactions; }
	Int_t GetNbOfElastics()const{ return nbOfElastics; }
	Int_t GetNbOfInteractions()const{ return nbOfInteractions; }
	void SetEnergyEnterScinti(Double_t EkinScinti){energyEnterScinti_MeV = EkinScinti;}
	Double_t GetEnergyEnterScinti()const{return energyEnterScinti_MeV; }
	void SetEnergyExitScinti(Double_t EkinScinti){energyExitScinti_MeV = EkinScinti;}
	Double_t GetEnergyExitScinti()const{return energyExitScinti_MeV;}

	// order tracks accordong to timing information
	static bool compareHits(brMuonTrack* const &a, brMuonTrack* const &b);
	//friend std::ostream& operator<<(std::ostream &o,
	//		brNeutronTrack const &track);

	void SetMuonTriggerUp(bool trig){muonTrigUp=trig;}
	void SetMuonTriggerLow(bool trig){muonTrigLow=trig;}
	bool GetMuonTriggerUp()const{ return muonTrigUp;}
	bool GetMuonTriggerLow()const{ return muonTrigLow;}

	void SetMuonTriggerUpBelow(bool trig){muonTrigUpBelow=trig;}
	void SetMuonTriggerLowBelow(bool trig){muonTrigLowBelow=trig;}
	bool GetMuonTriggerUpBelow()const{ return muonTrigUpBelow;}
	bool GetMuonTriggerLowBelow()const{ return muonTrigLowBelow;}

	private:

	Int_t trackID;
	std::string initialProcessName ; //Name of the first process of the given track
	TString finalProcessName   ; //Name of the last process of the given track
	TString initialVolumeName  ; //Name of the volume where the track starts
	TString finalVolumeName    ; //Name of the volume where the track ends
	Double_t initialTime_s       ; //Global time when the first process occurs
 	Double_t finalTime_s         ; //Global time when the last process occurs
	Double_t initialEnergy_MeV     ; //The kinetic energy at the begin of the track
	Double_t finalEnergy_MeV       ; //The kinetic energy at the end of the track
	Double_t energyDeposit_MeV       ; //The kinetic energy at the end of the track
	Int_t    parentID          ; //The track ID of the parent track. If the track is a primary then it's zero.
	Double_t initialPositionX_m  ; //The position of the vertex of the track
	Double_t finalPositionX_m    ; //The position of the end of the track
	Double_t initialPositionY_m  ; //The position of the vertex of the track
	Double_t finalPositionY_m    ; //The position of the end of the track
	Double_t initialPositionZ_m  ; //The position of the vertex of the track
	Double_t finalPositionZ_m    ; //The position of the end of the track
	Double_t totalTrackLength_m  ; //The total track length of the track
	Double_t totalEnergy_MeV       ; //Energy loss of the track
	Int_t nbOfElastics         ; //Number of elastic scatterings of the track
	Int_t nbOfInteractions     ; //Number of interactions of the track
	Double_t energyEnterScinti_MeV; // Neutron kinetic energy when entering the scintillator volume
	Double_t energyExitScinti_MeV; //Muon Kinetic energy when exiting the scintillator volume
	bool muonTrigUp;
	bool muonTrigLow;
	bool muonTrigUpBelow;
	bool muonTrigLowBelow;
	ClassDef(brMuonTrack,8);


};

typedef std::vector<brMuonTrack*> brMuonTrackVector;

#endif /* BRMUONTRACK_HH_ */
