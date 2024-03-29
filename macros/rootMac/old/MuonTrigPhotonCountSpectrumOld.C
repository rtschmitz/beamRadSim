#include "TCanvas.h"
#include "TTree.h"
#include "TGaxis.h"
#include "TStyle.h"
#include "TFile.h"
#include <iostream>
#include "TMath.h"
#include "TChain.h"
#include "/home/schmitz/sim/HannahTestStand/include/eurROOTEvent.hh"
#include "TGraph.h"
#include "TVector.h"
#include "TVectorF.h"
#include "TH1.h"
#include "TH1F.h"
using namespace std;


void MuonTrigPhotonCountSpectrumOld(){

//retreive file

  TChain rootEvents("Events");
  rootEvents.Add("/data/cdms1/schmitz/attLengTest/2p5mAttLeng/5cm/EURECA.root");
  rootEvents.Add("/data/cdms1/schmitz/attLengTest2p5mPS_wideBeam/5cm/EURECA.root");
  std::cout << "Reading file..." << std::endl;
  eurROOTEvent* myROOTEvent = new eurROOTEvent();
  rootEvents.SetBranchAddress("ROOTEvent", &myROOTEvent);
  Long64_t nentries=rootEvents.GetEntries();

//initialize variables
  double EDep=0;
  double h=4.135668*pow(10,-15);
  double c=3*pow(10,8);
  double wavelength;
  double final_z=0;
  double final_x=0;
  double final_y=0;
  double XYMag=0;
  int fiberInc=0;
  int under=0;
  int PECount=0;
  bool wls_conv  = false;
vector<double> wave;
int pmtCount=0;
int photonCount=0;
int numPhotons=0;
int numPhotonTracks=0;
vector<int> muonPhotons;
vector<int> muonPhotonsWide;
bool trig = false;
int count = 0;
//get data in file
  for (Long64_t i=0;i<nentries;i++) {
	//my issue may have to do with this
	rootEvents.GetEntry(i);
	if (i<1000){	
		numPhotonTracks=myROOTEvent->GetPhotonTracks()->size();
		muonPhotons.push_back(numPhotonTracks);
		if(i%100==0) cout << "Finished analyzing event " << i << " of 3000" << endl;
	}
	else if (i<2000){
		numPhotonTracks=myROOTEvent->GetPhotonTracks()->size();
		muonPhotonsWide.push_back(numPhotonTracks);
		if(i%100==0) cout << "Finished analyzing event " << i << " of 3000" << endl;
	}
   }

TH1I* hmphot = new TH1I("hmphot","",200,0,100000);
TH1I* hmphotWide = new TH1I("hmphotWide","",200,0,100000);
for (int j=0; j<muonPhotons.size(); j++){
	hmphot->Fill(muonPhotons[j]);
	hmphotWide->Fill(muonPhotonsWide[j]);
}

//hmphot->GetXaxis()->SetTitle("Number of Muon-Generated Photons");
//hmphot->GetYaxis()->SetTitle("Bin Members");
//hmphot->SetTitle("Photons Directly Generated by Incident Muons (Real distribution)");

hmphot->SetLineColor(kBlue);
hmphotWide->SetLineColor(kRed);

//TCanvas *c1 = new TCanvas("c1","c1",0,400,600,300);
hmphot->Draw("same");
hmphotWide->Draw("same");

muonPhotons.clear();
muonPhotonsWide.clear();
}
	
