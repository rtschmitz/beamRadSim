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


void MuonTrigPhotonCountSpectrumNew(){

//retreive file

  TChain rootEvents("Events");
  rootEvents.Add("/data/cdms1/schmitz/attLengTest2p5mPS_realBeam/5cm/EURECA.root");
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
vector<int> muonPhotonsReal;
bool trig = false;
int count = 0;
//get data in file
  for (Long64_t i=0;i<nentries;i++) {
	//my issue may have to do with this
	rootEvents.GetEntry(i);
		trig = myROOTEvent->GetMuonTracks()->at(0)->GetMuonTrigger();
		if (trig){
			numPhotonTracks=myROOTEvent->GetPhotonTracks()->size();
			muonPhotonsReal.push_back(numPhotonTracks);
			count++;
			if(count%100==0) cout << "Finished analyzing event " << 2000+count << " of 3000" << endl;
			if (count==1000) break;
		}
   }

TH1I* hmphotReal = new TH1I("hmphotReal","",200,0,100000);
for (int j=0; j<muonPhotonsReal.size(); j++){
	hmphotReal->Fill(muonPhotonsReal[j]);
}

hmphotReal->SetLineColor(kGreen);

hmphotReal->GetXaxis()->SetTitle("Number of Muon-Generated Photons");
hmphotReal->GetYaxis()->SetTitle("Bin Members");
hmphotReal->SetTitle("Photons Directly Generated by Incident Muons");

TCanvas *c1 = new TCanvas("c1","c1",0,400,600,300);
hmphotReal->Draw();

muonPhotonsReal.clear();

}	
