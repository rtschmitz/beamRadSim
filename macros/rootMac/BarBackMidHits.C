#include "TCanvas.h"
#include "TTree.h"
#include "TGaxis.h"
#include "TStyle.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include "TMath.h"
#include "TChain.h"
#include "/media/ryan/Storage/computing/mqFullSim/include/mqROOTEvent.hh"
#include "TGraph.h"
#include "TVector.h"
#include "TVectorD.h"
#include "TVectorF.h"
#include "TH1.h"
#include "TH1F.h"
#include "TString.h"
#include "TChain.h"
#include "TMultiGraph.h"
using namespace std;

//this version is used for triggering on single bar (and letting either of the middle 2 be the trigger). If triggering on the entire layer, use BarBackHits
void BarBackMidHits(){

//retreive file

  TChain rootEvents("Events");
  TString fileDir = "/media/ryan/Storage/computing/mqFullSim/data/muonRockTest/triggerOnBackMid/";
  TString textFilepath = "/media/ryan/Storage/computing/mqFullSim/data/";
  TString xTitle = "Number of Muon-Generated Photons";
  rootEvents.Add(fileDir+"MilliQan.root");
  std::cout << "Reading file..." << std::endl;
  mqROOTEvent* myROOTEvent = new mqROOTEvent();
  rootEvents.SetBranchAddress("ROOTEvent", &myROOTEvent);
  Long64_t nentries=rootEvents.GetEntries();

//initialize variables
int numPMTHits=0;
int hitNumberScan = 0;
int count = 0;
int hitNumberCount = 0;
int barBackMidHits=0;
std::ifstream barHitData;
///*

vector<int> evtNum;
vector<int> trackID;
vector<int> barNum;
vector<std::string> particleName;
vector<Double_t> particleEnergy;
vector<Double_t> evtTime;
vector<Int_t> muonHits;
vector<Double_t> muonHitsPrimaryBar; //first bar hit
vector<Int_t> muonHitsSecondaryBar; //other bar in plane in layer
vector<Int_t> muonHitsUpper; //hits in the above pair in back layer
vector<Int_t> muonHitsLower; //hits in the lower pair in back layer
vector<Int_t> muonHitsFront; //hits in the front layer
vector<Int_t> muonHitsMid; //hits in the mid layer
vector<Double_t> muonHitsBack; //total hits in the back layer
vector<Double_t> timeSep; //time separation between two distinct gamma hits

int mPrim=0;
int mSec=0;
int mUpper=0;
int mLower=0;
int mFront=0;
int mMid=0;
int mBack=0;

/*
vector<Double_t> muonHitsGamma;
vector<Double_t> muonHitsNeutron;
vector<Double_t> muonHitsElectron;
vector<Double_t> muonHitsOther;
*/

vector<vector<Int_t>> PMTHitNumVec;
//*/
//int evtNum[lines];
//std::string particleName[lines];
//double particleEnergy[lines];
//double evtTime[lines];
int a,b,c,ignore;
std::string d;
double e,f;
int lines=0;
Int_t hitN=0;
int evtNumTemp=0;
bool uniqueHit=false;

barHitData.open(textFilepath+"particleHitData.txt");
while(true){
	barHitData >> a >> b >> c >> d >> e >> f >> ignore;
	if(barHitData.eof()) break;
//	if(a>5000) break; //for debugging
	evtNum.push_back(a);
	trackID.push_back(b);
	barNum.push_back(c);
	particleName.push_back(d);
	particleEnergy.push_back(e);
	evtTime.push_back(f);
	cout << "Event read in: " << a << "	" << b << "	" << c << "	" << d << "	" << e << "	" << f << endl;
	lines++;
}
barHitData.close();

std::cout << "Number of lines in particleHitData.txt: " << lines << std::endl;
std::cout << "Last particle to hit detector is " << particleName[lines-1] << endl;

std::ofstream barWithPE;
barWithPE.open(textFilepath+"particleHitDataWithPE.txt");
//get data in file
  for (Long64_t i=0;i<nentries;i++) {
//  for (Long64_t i=0;i<5000;i++) { //for debugging

  	  rootEvents.GetEntry(i);
	
		//get number of hits in the target region
		hitNumberScan = myROOTEvent->GetBarHit();
		
		//look at the event if we had hits
		if(hitNumberScan > 0 && barNum[hitNumberCount]==16){

			//get number of hits in PMTs anywhere
			numPMTHits=myROOTEvent->GetPMTRHits()->size();

			//check if we only had one unique hit
			if(hitNumberScan==1) uniqueHit=true;
			if(hitNumberScan>1){
				for(int p=hitNumberCount;p<hitNumberCount+hitNumberScan-1;p++){
					if(trackID[p]==trackID[p+1]) uniqueHit=true;
					else {uniqueHit=false; break;}
				}
			}
			//if we had one unique particle hit the region
			if(uniqueHit){

			//loop over PMT Hits and store hits in different regions into parameters for this event
			for(int h=0;h<numPMTHits;h++){
				hitN=myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();
				if(hitN==barNum[hitNumberCount]) mPrim++;
				else if(hitN==13||hitN==16) mSec++;
				else if(hitN==14||hitN==17) mUpper++;
				else if(hitN==12||hitN==15) mLower++;
				else if(hitN>5 && hitN<12) mMid++;
				else if(hitN<6) mFront++;
			}
			
			///////////////////////verbose output //////////////////////////////
cout << "Bar Hit Info: " << evtNum[hitNumberCount] << " " << particleName[hitNumberCount] << " " << particleEnergy[hitNumberCount] << " " << evtTime[hitNumberCount] << " " << endl;
cout << "Total	Primary	Secondary	Upper	Lower	BackLayer	MidLayer	FrontLayer" << endl;
cout << numPMTHits << "	" << mPrim << "	" << "	" << mSec << "	" << mUpper << "	" << mLower << "	" << mPrim+mSec+mUpper+mLower << "	" << mMid << "	" << mFront << endl;
		
		barWithPE << evtNum[hitNumberCount] << "	" << particleName[hitNumberCount] << "	" << particleEnergy[hitNumberCount] << "	" << evtTime[hitNumberCount] << "	"
			<< numPMTHits << "	" << mPrim << "	" << mSec << "	" << mUpper << "	" << mLower << "	" << (mPrim+mSec+mUpper+mLower) << "	" << mMid << "	" << mFront << endl;
			////////////////////////////////////////////////////////////////////
		
		mBack=mPrim+mSec+mUpper+mLower;

		muonHits.push_back(numPMTHits);
		muonHitsPrimaryBar.push_back(mPrim);
		muonHitsSecondaryBar.push_back(mSec);
		muonHitsUpper.push_back(mUpper);
		muonHitsLower.push_back(mLower);
		muonHitsBack.push_back(mBack);
		muonHitsMid.push_back(mMid);
		muonHitsFront.push_back(mFront);

		mPrim=0;
		mSec=0;
		mUpper=0;
		mLower=0;
		mMid=0;
		mFront=0;

		}
			else {cout << hitNumberScan << " is more than 1 hit! We had a multiple shower." << endl;
				cout << "multiple particles which hit detector: ";
				for(int n=hitNumberCount;n<hitNumberCount+hitNumberScan;n++) cout << particleName[n] << "	" << endl;

			cout << "Looking at hit timing information..." << endl;
				for(int n=hitNumberCount;n<hitNumberCount+hitNumberScan;n++){
					if(trackID[hitNumberCount]!=trackID[n]){
						timeSep.push_back(evtTime[n]-evtTime[hitNumberCount]);
					}
				}
		}
//			if(i%1000==0) cout << "Finished analyzing event number " << i << endl;
   }
		hitNumberCount+=hitNumberScan;
}
//create some datastructures to fill up the histograms
vector<Double_t> nPEfrac;

TVectorD tmuonHitsPrimaryBar(muonHitsPrimaryBar.size(), &muonHitsPrimaryBar[0]);
TVectorD tmuonHitsBack(muonHitsBack.size(), &muonHitsBack[0]);
TVectorD ttimeSep(timeSep.size(), &timeSep[0]);

int nbins=50;

//create histograms to look at numbers of hits in the PMT compared to the back layer
TH1D* hPrimaryHit = new TH1D("hPrimaryHit","",nbins,0,tmuonHitsBack.Max()*(1+(double)1/nbins));
hPrimaryHit->GetXaxis()->SetTitle("Number of PE detected in Primary bar");
hPrimaryHit->GetYaxis()->SetTitle("Bin Members");
hPrimaryHit->SetLineColor(kRed);

TH1D* hBackHit = new TH1D("hBackHit","",nbins,0,tmuonHitsBack.Max()*(1+(double)1/nbins));
hBackHit->GetXaxis()->SetTitle("Number of PE detected in Primary bar");	
hBackHit->GetYaxis()->SetTitle("Bin Members");

TH1D* hHitFrac = new TH1D("hHitFrac","",nbins,0,(1+(double)1/nbins));
hHitFrac->GetXaxis()->SetTitle("Percentage of hits in trigger bar");	
hHitFrac->GetYaxis()->SetTitle("Bin Members");

//fill the histograms
int muonHitsPrimaryEmpty=0;
for(int m=0;m<muonHitsPrimaryBar.size();m++){
	if(muonHitsPrimaryBar[m]>0){
	      	nPEfrac.push_back((double)muonHitsPrimaryBar[m]/muonHitsBack[m]);
		hHitFrac->Fill(nPEfrac[m]);
	}
	else {nPEfrac.push_back(-1); muonHitsPrimaryEmpty++;}
	
	hPrimaryHit->Fill(muonHitsPrimaryBar[m]);	
	hBackHit->Fill(muonHitsBack[m]);
}

cout << "Fraction of trigger region hits that were empty: " << (double)muonHitsPrimaryEmpty/muonHitsPrimaryBar.size() << endl;

// create time difference plot and fill it
TH1D* hTimeDiff = new TH1D("hTimeDiff","",nbins,0,ttimeSep.Max()*(1+(double)1/nbins));
hTimeDiff->GetXaxis()->SetTitle("Time difference between Distinct Particle Hits (ns)");
hTimeDiff->GetYaxis()->SetTitle("Bin Members");
for(m=0;m<timeSep.size();m++) hTimeDiff->Fill(timeSep[m]);

TVectorD tnPEfrac(nPEfrac.size(), &nPEfrac[0]);

///*
//create graph: primary hits vs. fraction of hits in back layer
TGraph *hPEfrac = new TGraph(tmuonHitsPrimaryBar,tnPEfrac);
hPEfrac->SetMarkerColor(1); //black
hPEfrac->SetMarkerStyle(kFullDotMedium);
hPEfrac->GetXaxis()->SetTitle("Number of PE detected in trigger bar");
hPEfrac->GetYaxis()->SetTitle("Fraction of PE detected in trigger bar");

//remove the points where there was no nPE registered in the trigger bar
/*
double x,y;
for(m=0;m<nPEfrac.size();m++){
        hPEfrac->GetPoint(m,x,y);
        if(y==-1) {
		hPEfrac->RemovePoint(m);
		m--;
	}
}
*/
//plot stuff
TCanvas* c1 = new TCanvas("c1","c1",0,400,600,300);
hPEfrac->Draw("AP");
//*/
TCanvas* c2 = new TCanvas("c2","c2",0,400,600,300);
hPrimaryHit->Draw();

TCanvas* c3 = new TCanvas("c3","c3",0,400,600,300);
hBackHit->Draw();

TCanvas* c4 = new TCanvas("c4","c4",0,400,600,300);
hHitFrac->Draw();

TCanvas* c5 = new TCanvas("c5","c5",0,400,600,300);
hTimeDiff->Draw();

//clean up memory
muonHitsPrimaryBar.clear();
muonHitsSecondaryBar.clear();
muonHitsUpper.clear();
muonHitsLower.clear();
muonHitsBack.clear();
muonHitsMid.clear();
muonHitsFront.clear();
nPEfrac.clear();
evtNum.clear();
particleName.clear();
particleEnergy.clear();
evtTime.clear();
trackID.clear();
barNum.clear();
}
