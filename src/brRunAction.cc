#ifdef G4ANALYSIS_USE
#include "brAnalysisManager.hh"
#endif

#include <time.h>

#include "brRunAction.hh"
#include "brRunActionMessenger.hh"
#include "brHistoManager.hh"
#include "brUserEventInformation.hh"
#include "Randomize.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4IsotopeVector.hh"
#include "G4MaterialPropertiesTable.hh"
#include "TRandom3.h"
char filename[200];


brRunAction::brRunAction(brHistoManager* histo) :
	histoManager(histo){

	sprintf(filename,"Sim_%d");
	DataFileNamePrefix = G4String(filename);

	runMessenger = new brRunActionMessenger(this);

}
/**
 * @brief	Generic deconstructor
 */
brRunAction::~brRunAction() {
}

/**
 * @brief      Generate the run, see Geant4 9.3 Manual for Application Developers, p.198
 * @return     The current G4Run
 **/
G4Run* brRunAction::GenerateRun() {
	//Don't want to create my own G4run, but want to change the seeds of the random generator.
	//If I do this in the Begin OfRunAction then I get errors caused by inconsistent timing during the run, e.g. steps ends before it starts.
	//Maybe it's caused because the initial run conditions are already set and the change of the seeds after this leads to the problem.
	//->Acc. to manual GenerateRun is called before the run is create, so try the change of the seeds here
	//according to Markus Horn: Generate the initial seeds from the system time
	//TODO: fix problem with verbose level printouts
	G4long seeds[2];
	G4long Seed;
	time_t systime = time(NULL);
	TRandom3 *rand = new TRandom3(0); //Acc. to class documentation of TRandom3: 'the seed is guaranteed to be unique in space and time'.
	Seed = (G4long) systime;
	seeds[0] = (G4long) (systime * rand->Rndm());
	seeds[1] = (G4long) (systime / rand->Rndm());
	G4Random::setTheSeed(Seed);
	G4Random::setTheSeeds(seeds);

	G4Random::showEngineStatus();
	//For the generation of the actual run used the Geant4 standard method by calling the method of the base class
	return (G4UserRunAction::GenerateRun());
}

/**
 * @brief	Set the initial seeds of the random generator at the begin of the run.
 * @parameter	aRun	Pointer to the current run, which just started.
 */
void brRunAction::BeginOfRunAction(const G4Run* aRun) {

	// book ROOT histos
	histoManager->book(DataFileNamePrefix);

	//print some information if wanted
	//if (verboseLevel > 1) {
	 G4int myRunID = aRun->GetRunID();
	 G4cout << ">Begin run " << myRunID << G4endl;
	 //}
}


/**
 * @brief	Passed a pointer to the finished run to the brAnalysisManager.
 * @parameter	aRun	Pointer to the current run, which just finished.
 */
void brRunAction::EndOfRunAction(const G4Run* aRun) {

	//save ROOT histos

	histoManager->save();

	 //if(recorder) recorder->RecordEndOfRun(aRun);
	//TODO: fix problem with verbose level
	//if(verboseLevel > 1){
	 G4cout << "<End run" << G4endl;
	 //}
}



