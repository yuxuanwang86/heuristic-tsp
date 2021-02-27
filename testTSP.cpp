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
	tsp.setLSparameters(20,1);

    tsp.readInstanceFromFile(instanceFile);
    //tsp.displayInstance();
    //tsp.naiveInit();
    //tsp.greedyInit();
    tsp.randomizedConstr();
    //tsp.randomizedGreedy();
    //tsp.hillClimbing(false,false,true);
    tsp.simulatedAnnealing();

    // vector<int> vec;
    // for(int i = 0; i < 10; i++) vec.push_back(i);
    // vec.insert(vec.begin() + 1 + 1, vec[0]);
    // vec.erase(vec.begin());
    // vec.resize(10);
    // vector<int>::const_iterator first = vec.begin();
    // vector<int>::const_iterator last = vec.begin() + 3 + 1;
    // vector<int> newVec(first, last);
    // for(auto it: newVec) cout<<it<<' ';

    return 0;

}

