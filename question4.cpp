/********** testTSP.cpp **********/

#include "solverTSP.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int getStrSub(string str,char ch){

	int result = -1;

	for(int i=0;i<str.length();i++){
		if(str[i] == ch){
			result = i;
		}
	}

	return result;
}
int getResult(string filename){
    ifstream fin("./result/resultatsATSP.csv"); 
    string line;
    while (getline(fin, line))                          
    {
        string::size_type idx;
          //find the position of ;
        int pos = getStrSub(line,';');
        string strpart = line.substr(0,pos);
       
        int numberpart = stoi(line.substr(pos+1));
        idx = filename.find(strpart) ;
        if(idx!=string::npos){
            return numberpart;
            break;
        }
    }
    return 0;
}
int main(int argc, char** argv) {
    if (argc < 2) {
        //cerr << "Usage: tsp inputFile " << endl;
        return 1;
    }

    const char* instanceFile = argv[1];

    //get the result from csv file
    string filename = argv[1];
    int result = getResult(filename);
    

    SolverTSP tsp;
    vector<pair<string,vector<double>>> tableau; 
    tsp.setVerboseMode(true);
	tsp.setLSparameters(10,1);

    tsp.readInstanceFromFile(instanceFile);
    //tsp.displayInstance();

    tsp.naiveInit();
    int naiveCost = tsp.getSolutionCost();
    vector<double> naiveInitdata ;
    naiveInitdata.push_back((double)naiveCost);
    naiveInitdata.push_back((double)naiveCost - result);
    tableau.push_back(make_pair("NaiveInit",naiveInitdata));


    tsp.greedyInit();
    int greedyCost = tsp.getCurrentSolutionCost();
    vector<double> greedyInitdata;
    greedyInitdata.push_back( (double)greedyCost);
    greedyInitdata.push_back( (double)greedyCost - result);
    tableau.push_back(make_pair("GreedyInit",greedyInitdata));

  

    //randomized Constr part result
    double medConstr =0;
    for(int i =0 ;i< 20; i++){
        tsp.randomizedConstr();
        medConstr+=tsp.getCurrentSolutionCost();
    }
    medConstr = medConstr / 10;
    // cout<< medConstr<<endl;
    vector<double> randomizedConstrPart;
    randomizedConstrPart.push_back(medConstr);
    randomizedConstrPart.push_back(medConstr - result);
    tableau.push_back(make_pair("randomizedConstr",randomizedConstrPart));

    //randomized greedy part result
    double medRandGred =0;
    for(int i =0 ;i< 10; i++){
        tsp.randomizedGreedy();
        medRandGred+=tsp.getCurrentSolutionCost();
    }
    medRandGred = medRandGred / 10;
    // cout<< medRandGred<<endl;
    vector<double> randomizedGredPart;
    randomizedGredPart.push_back(medRandGred);
    randomizedGredPart.push_back(medRandGred - result);
    tableau.push_back(make_pair("randomizedGreedy",randomizedGredPart));

    cout<< "Heuristique_type"<<"\t"<<"CurrentSolution"<<"\t"<<"surcout"<<endl;
     for(int i = 0; i < tableau.size(); ++i) {
        std::cout <<tableau[i].first << "\t";
        for(int j = 0; j<2;j++){
            cout<<tableau[i].second[j]<< "\t";
        }
        cout<<endl;
    }
    
    return 0;

}

