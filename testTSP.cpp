/********** testTSP.cpp **********/

#include "solverTSP.hpp"


using namespace std;


int main(int argc, char** argv) {
    if (argc < 2) {
        //cerr << "Usage: tsp inputFile " << endl;
        return 1;
    }

    const char* instanceFile = argv[1];

    SolverTSP tsp;
    tsp.setVerboseMode(true);
	tsp.setLSparameters(10,1);

    tsp.readInstanceFromFile(instanceFile);
    tsp.displayInstance();
    //tsp.naiveInit();
    tsp.greedyInit();
    //tsp.hillClimbing(true,true,true);

    return 0;

}

