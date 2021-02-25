/********** testTSP.cpp **********/

#include "solverTSP.hpp"
#include <iostream>

using namespace std;


int main(int argc, char** argv) {
    if (argc < 2) {
        //cerr << "Usage: tsp inputFile " << endl;
        return 1;
    }

    const char* instanceFile = argv[1];

    SolverTSP tsp;
    tsp.setVerboseMode(true);
	tsp.setLSparameters(2,1);

    tsp.readInstanceFromFile(instanceFile);
    tsp.displayInstance();
    tsp.naiveInit();
    tsp.greedyInit();
    tsp.randomizedConstr();
    tsp.randomizedGreedy();
    //tsp.hillClimbing(false,false,true);

    // vector<int> vec;
    // for(int i = 1; i <= 10; i++) vec.push_back(i);
    // vec.insert(vec.begin() + 1 + 1, vec[0]);
    // vec.erase(vec.begin());
    // vec.resize(10);
    // for(auto it: vec) cout<<it<<' ';

    return 0;

}

