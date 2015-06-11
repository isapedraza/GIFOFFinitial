// Example to build a Tree with integers and vectors.
// It generates a ROOT file with a TTree with two Integers and two Vectors.
// Author: Isabel Pedraza
// Created on Jun 8, 2015

#include <iostream>                 // class for input/output streams
#include <stdio.h>                  // printf, scanf, puts, NULL
#include <stdlib.h>                 // srand, rand, atof
#include <time.h>                   // time
#include <fstream>                  // class for input/output file streams

#include <string.h> 				  // class to manipulate strings
#include <sstream>                    // class for parsing strings
#include <vector>

#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include <TString.h>

using namespace std;


std::string ExtractDirectory( const std::string& path )
{
	return path.substr( 0, path.find_last_of( '/' ) +1 );
}

std::string ExtractFilename( const std::string& path )
{
	return path.substr( path.find_last_of( '/' ) +1 );
}

std::string ChangeExtension( const std::string& path, const std::string& ext )
{
	std::string filename = ExtractFilename( path );
	return ExtractDirectory( path ) +filename.substr( 0, filename.find_last_of( '.' ) ) +ext;
}

//void readfile(TString file="/Users/isabel/Documents/BUAP/CMS/RPCs/GIF2015/SourceON_PK-140_Double_B2_Random_220mV_9200V_run20150609092921.dat"){
void readfile(TString file="/Users/isabel/Documents/BUAP/CMS/RPCs/GIF2015/tail.dat"){
	//variables to read the lines of the file
	ifstream inp ;
	inp.open(file);
	string line;

	int i=1;
	int hits=-1;

	//variables to read in the file
	double EventNumber=-1;
	double number_of_hits=-1;

	vector<double>   *TDC_channel     = new vector<double>;
	vector<double>   *TDC_TimeStamp   = new vector<double>;
	TDC_channel->clear();
	TDC_TimeStamp->clear();

	while(getline(inp,line)) {
		stringstream ss(line);
		string item;
		i=1;
		if(hits==number_of_hits) hits=-1;


		while (getline(ss, item,'\t')) {
			switch (i){
			case 1: if(hits==-1) EventNumber=atof(item.c_str());
			else TDC_channel->push_back(atof(item.c_str()));
			//if(hits==-1) cout << "entra en el evento " << item << endl;
			//else cout << "TDC Channel " << item << endl;
			break;
			case 2: if(hits==-1) number_of_hits=atof(item.c_str());
			else TDC_TimeStamp->push_back(atof(item.c_str()));
			//if(hits==-1) cout << "entra en el numbero de hits " << item <<  endl;
			//else cout << "TDC TimeStamp " << item << endl;
			break;
			default:
				break;
			}
			i++;
		}

		if(number_of_hits>0)
			hits++;

	}
	inp.close();
}

int main(int argc, char *argv[]){

	//readfile();

	if (argc==2 && TString(argv[1])=="h") {
		printf("\nUsage: %s [File list or File] \n",argv[0]);
		exit(0);
	}

	TString  INFILE   = argc>1? argv[1]:"/Users/isabel/Documents/BUAP/CMS/RPCs/GIF2015/tail.dat";
	string    sfile   = INFILE.Data();
	TString  filename=ExtractFilename(ChangeExtension(sfile,""));

	TFile *f = new TFile(filename+".root", "recreate");
	TTree *T = new TTree("CollectionTree","test");

	Int_t           EventNumber=-1;
	Int_t           number_of_hits=-2;
	vector<Int_t>   *TDC_channel     = new vector<Int_t>;
	vector<Long64_t>   *TDC_TimeStamp   = new vector<Long64_t>;

	T->Branch("EventNumber",     &EventNumber,  "EventNumber/I"  );
	T->Branch("number_of_hits",   &number_of_hits,"number_of_hits/I");
	T->Branch("TDC_channel",      &TDC_channel);
	T->Branch("TDC_TimeStamp",         &TDC_TimeStamp        );

	//Variables to read the file
	ifstream inp ;
	inp.open(INFILE);
	string line;
	int i=1;
	int hits=-1;

	TDC_channel->clear();
	TDC_TimeStamp->clear();

	while(getline(inp,line)) {
		stringstream ss(line);
		string item;
		i=1;
		if(hits==number_of_hits) {
			T->Fill();
			hits=-1;
			TDC_channel->clear();
			TDC_TimeStamp->clear();
		}

		while (getline(ss, item,'\t')) {
			switch (i){
			case 1: if(hits==-1) EventNumber=atof(item.c_str());
			else TDC_channel->push_back(atof(item.c_str()));
			//if(hits==-1) cout << "entra en el evento " << item << endl;
			//else cout << "TDC Channel " << item << endl;
			break;
			case 2: if(hits==-1) number_of_hits=atof(item.c_str());
			else TDC_TimeStamp->push_back(atof(item.c_str()));
			//if(hits==-1) cout << "entra en el numbero de hits " << item <<  endl;
			//else cout << "TDC TimeStamp " << item << endl;
			break;
			default:
				break;
			}
			i++;
		}

		if(number_of_hits>0)
			hits++;

	}
	inp.close();

	T->Fill();
	T->Print();
	f = T->GetCurrentFile();
	f->Write(0, TObject::kWriteDelete);

	delete f;

	cout << "Isa's CODE successfully completed " << endl;

}
