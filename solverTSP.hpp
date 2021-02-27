
#ifndef INSTANCETSP_HPP
#define INSTANCETSP_HPP

#include <vector>
#include <string>
#include <climits>

using namespace std;

class SolverTSP {

private:

    //parameters related to the input data
    // Number of cities
    int nbCities;
    // Vector/matrix of distances between two cities
    vector<int>  distanceMatrix;

    //related to the solution of instance

    vector<int> curSol;
    vector<int> revSol;
	int currentSolutionCost;
    int revSolutionCost;
    int subSolutionCost;

    vector<int>  bestSolution;
	int bestSolutionCost;

	//parameters of LocalSearch
	int nbMaxIterLS;
	int nbMaxRestart;

	int nbIterLS;
	bool verboseMode;

    //private methods, only used by solving methods
	
	//WARNING: asymmetric TSP, distance from city i1 to i2, may be different with i2 -> i1
    int distance(int i1,int i2) { return distanceMatrix[nbCities * i1+i2]; };
    int distSol(int i1,int i2) { return distanceMatrix[nbCities * curSol[i1]+curSol[i2]]; };
    // version pour ne pas se soucier des bords?
    //    int distSol(int i1,int i2){return distanceMatrix[nbCities * curSol[i1%nbCities]+curSol[i2%nbCities]];};

    void printStatus();
    bool checkFeasibility();

    bool hillClimbingIter(bool swapMoves, bool revMoves, bool insertMoves);//TODO

    void computeReversedDistanceCost();
    void reverseCities();

public:

    //public methods, to call in external test functions

    void updateBestSolution();
    void computeDistanceCost();
    int getCurrentSolutionCost() { return currentSolutionCost; };


    void setLSparameters(int nbIter, int nbRestart){nbMaxIterLS = nbIter; nbMaxRestart= nbRestart;};
    void setVerboseMode(bool b){verboseMode=true;};
    int getSolutionCost(){updateBestSolution(); return bestSolutionCost;};
    int getNbIterLS(){return nbIterLS;};

    //solving methods
    void hillClimbing(bool swapMoves, bool revMoves, bool insertMoves);

    void naiveInit();
    void greedyInit();

    void randomizedConstr();
    void randomizedGreedy();
    void graspHC(bool swapMoves, bool revMoves, bool insertMoves);//TODO

    void simulatedAnnealing();//TODO


    /* Reads and displays instance data. */
    void readInstanceFromFile(const string& fileName);
    void displayInstance();

};

#endif
