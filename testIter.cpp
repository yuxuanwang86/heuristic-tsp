
#include "solverTSP.hpp"
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;

void printNaiveInitResult(SolverTSP & tsp, string sep, ofstream & outfile){
    auto start = std::chrono::steady_clock::now();
    tsp.naiveInit();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    tsp.updateBestSolution();

    outfile << sep << tsp.getSolutionCost() << sep << elapsed_seconds.count() << sep;

}

void printHillClimbingResult(SolverTSP & tsp, string sep, ofstream & outfile){
    auto start = std::chrono::steady_clock::now();
    tsp.hillClimbing(true,true,true);

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    tsp.updateBestSolution();
    outfile << sep << tsp.getSolutionCost() << sep << tsp.getNbIterLS() << sep << elapsed_seconds.count() << sep;

}

int main(int argc, char** argv) {


	vector<string> files {
		"instances/br17.atsp",
		"instances/ft53.atsp",
		"instances/ft70.atsp",
		"instances/ftv33.atsp",
		"instances/ftv35.atsp",
		"instances/ftv38.atsp",
		"instances/ftv44.atsp",
		"instances/ftv47.atsp",
		"instances/ftv55.atsp",
		"instances/ftv64.atsp",
		"instances/ftv70.atsp",
		"instances/ftv170.atsp",
		"instances/kro124p.atsp",
		"instances/p43.atsp",
		"instances/rbg323.atsp",
		"instances/rbg358.atsp",
		"instances/rbg403.atsp",
		"instances/rbg443.atsp",
		"instances/ry48p.atsp",

	};

    SolverTSP tsp;
	tsp.setLSparameters(1000,1);


    const char* instanceFile;
    string sep = ";";

    ofstream outfile("csv/initNaive.csv", ios::out | ios::trunc);
    if(outfile)
    {
        // instructions
    	for (std::vector<string>::iterator it = files.begin() ; it != files.end(); ++it){
    		instanceFile = (*it).c_str();
    	    outfile << *it << sep;
    	    tsp.readInstanceFromFile(instanceFile);
    	    printNaiveInitResult(tsp,sep,outfile);
    	    printHillClimbingResult(tsp,sep,outfile);
    	    outfile << endl;

    	}
    	outfile.close();
    }





    return 0;

}


