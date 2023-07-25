/*
 * brPhysicsList.cc
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: Shielding.icc 76249 2013-11-08 11:21:29Z gcosmo $
//
//---------------------------------------------------------------------------
//
// ClassName:
//
// Author: 2010 Tatsumi Koi, Gunter Folger
//
//   created from FTFP_BERT
//
// Modified:
// 16.08.2010 H.Kurashige: Remove inclusion of G4ParticleWithCuts
// 26.04.2011 T.Koi: Add G4RadioactiveDecayPhysics
// 16.10.2012 A.Ribon: Use new default stopping
// 07.11.2013 T.Koi: Add IonElasticPhysics, Set proton cut to 0 to generate
//                   low energy recoils and activate production of fission
//                   fragments
//
//----------------------------------------------------------------------------
//
 */

#include "brShieldingPhysicsList.hh"

////

#include <iomanip>

#include "globals.hh"
#include "G4ios.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmProcessOptions.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonQMDPhysics.hh"
#include "G4IonElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsLEND.hh"

#include "G4DataQuestionaire.hh"
#include "G4HadronPhysicsShielding.hh"

#include "G4OpticalPhysics.hh"
//#include "G4OpticalProcessIndex.hh"


//template<class T> TShielding<T>::TShielding(G4int ver):  T()
//template<class T> TShielding<T>::TShielding( G4int verbose, G4String LEN_model ):  T()
brShieldingList::brShieldingList( G4int verbose, G4String LEN_model )
{
  // default cut value  (1.0mm)
  // defaultCutValue = 1.0*CLHEP::mm;
  G4DataQuestionaire it(photon, neutron, radioactive);
  G4cout << "<<< Geant4 Physics List simulation engine: Shielding 2.1"<<G4endl;
  G4cout <<G4endl;
//  this->defaultCutValue = 0.01*CLHEP::um; //used 0.01mm in smallStep run
//  this->defaultCutValue = 0.7*CLHEP::mm; //used 0.01mm in smallStep run
  this->defaultCutValue = 10*CLHEP::um; //used 0.01mm in smallStep run
  this->SetVerboseLevel(verbose);

 // EM Physics
  this->RegisterPhysics( new G4EmStandardPhysics_option4(verbose));
  //G4EmProcessOptions emOptions;
  //emOptions.SetFluo(true); // To activate deexcitation processes and fluorescence
  //emOptions.SetAuger(true); // To activate Auger effect if deexcitation is activated
  //emOptions.SetPIXE(true); // To activate Particle Induced X-Ray Emission (PIXE)

  // Synchroton Radiation & GN Physics
  this->RegisterPhysics( new G4EmExtraPhysics(verbose) );

  // Decays
  this->RegisterPhysics( new G4DecayPhysics(verbose) );
  //if ( rad == true ) this->RegisterPhysics( new G4RadioactiveDecayPhysics(verbose) );
  this->RegisterPhysics( new G4RadioactiveDecayPhysics(verbose) );
///*
  size_t find = LEN_model.find("LEND__");
  G4String evaluation;
  if ( find != G4String::npos )
  {
      evaluation=LEN_model;
      evaluation.erase(0,find+6);
      LEN_model="LEND";
  }

  // Hadron Elastic scattering
  if ( LEN_model == "HP" )
  {
     this->RegisterPhysics( new G4HadronElasticPhysicsHP(verbose) );
  }
  else if ( LEN_model == "LEND" )
  {
     this->RegisterPhysics( new G4HadronElasticPhysicsLEND(verbose,evaluation) );
     G4DataQuestionaire itt(lend);
  }
  else
  {
     G4cout << "Shielding Physics List: Warning!" <<G4endl;
     G4cout << "\"" << LEN_model << "\" is not valid for the low energy neutorn model." <<G4endl;
     G4cout << "Neutron HP package will be used." <<G4endl;
     this->RegisterPhysics( new G4HadronElasticPhysicsHP(verbose) );
  }

   // Hadron Physics
  G4HadronPhysicsShielding* hps = new G4HadronPhysicsShielding(verbose);
  if ( LEN_model == "HP" )
  {
     ;
  }
  else if ( LEN_model == "LEND" )
  {
     hps->UseLEND(evaluation);
  }
  else
  {
     //G4cout << "Shielding Physics List: Warning." <<G4endl;
     //G4cout << "Name of Low Energy Neutron model " << LEN_model << " is invalid." <<G4endl;
     //G4cout << "Will use neutron HP package." <<G4endl;
  }
  this->RegisterPhysics( hps );
  //this->RegisterPhysics( new G4HadronPhysicsShielding(verbose,lend));

  if ( LEN_model == "HP" ) {
     //Activate prodcuton of fission fragments in neutronHP
     char env_ff[]="G4NEUTRONHP_PRODUCE_FISSION_FRAGMENTS=1";
     putenv(env_ff);
  }
//*/
  // Stopping Physics
  this->RegisterPhysics( new G4StoppingPhysics(verbose) );

  // Ion Physics
  this->RegisterPhysics( new G4IonQMDPhysics(verbose));

  this->RegisterPhysics( new G4IonElasticPhysics(verbose));

  // Neutron tracking cut --> not by default
  // this->RegisterPhysics( new G4NeutronTrackingCut(verbose));

  // G.H.
  //optical physics added manually to shielding list
/*
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();


   opticalPhysics->SetScintillationYieldFactor(1.0);
   //opticalPhysics->SetScintillationExcitationRatio(0.0);

   //opticalPhysics->SetMaxNumPhotonsPerStep(300);
   //opticalPhysics->SetMaxBetaChangePerStep(10.0);

   opticalPhysics->SetTrackSecondariesFirst(kCerenkov,false);
   opticalPhysics->SetTrackSecondariesFirst(kScintillation,false);


   this->RegisterPhysics( opticalPhysics );
*/
}

brShieldingList::~brShieldingList()
{
}

void brShieldingList::SetCuts()
{
  if (this->verboseLevel >1){
    G4cout << "Shielding::SetCuts:";
  }
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types

  this->SetCutsWithDefault();

  //Set proton cut value to 0 for producing low energy recoil nucleus
  this->SetCutValue(0, "proton");

//  if (this->verboseLevel > 0)
//    G4VUserPhysicsList::DumpCutValuesTable();
}





