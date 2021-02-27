/********** testTSP.cpp **********/

#include "solverTSP.hpp"
#include <iostream>
#include <tuple>
#include<time.h>

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


    vector<string> heuristic_type;
    // tsp.simulatedAnnealing();
    vector<tuple<int,int,int>> combinaisons; // combinaisons for the different move modes
    vector<double> calculate_time;
    vector<int> nbiterations;
    vector<int> localminimaux;

    clock_t start,end;

    tuple<int, int, int> curr_comb ;
    
        for(int j=0;j<=1;j++){
            std::get<0>(curr_comb) = j;
            for(int j=0;j<=1;j++){
              std::get<1>(curr_comb) = j;
               for(int j=0;j<=1;j++){
                 std::get<2>(curr_comb) = j;
                 combinaisons.push_back(curr_comb);
                } 
             }  
        }

        combinaisons.erase(combinaisons.begin()); // delete (0,0,0)
  
        //think about how to calculate nb iteration, and the time of calculate
       cout<< tsp.getNbIterLS()<< endl;

    for(int x = 0; x<3; x++){
        for(int i=0;i<combinaisons.size();i++){ // there are only 3 heuristique types
       
            switch (x)
            {
            case 0: 
                /* code */
                    tsp.greedyInit();
                    start = clock();
                    tsp.hillClimbing(std::get<0>(combinaisons[i]),std::get<1>(combinaisons[i]),std::get<2>(combinaisons[i]));
                    end = clock();
                    localminimaux.push_back(tsp.getCurrentSolutionCost());
                    calculate_time.push_back(end-start);
                    heuristic_type.push_back("greedyInit");
                    nbiterations.push_back(tsp.getNbIterLS()); // nb iterations
                break;
            case 1:
                    tsp.randomizedConstr();
                    start = clock();
                    tsp.hillClimbing(std::get<0>(combinaisons[i]),std::get<1>(combinaisons[i]),std::get<2>(combinaisons[i]));
                    end = clock();
                    localminimaux.push_back(tsp.getCurrentSolutionCost());
                    calculate_time.push_back(end-start);
                    heuristic_type.push_back("randomizedConstr");
                    nbiterations.push_back(tsp.getNbIterLS()); // nb iterations
                break;

            case 2:
                    tsp.randomizedGreedy();
                    start = clock();
                    tsp.hillClimbing(std::get<0>(combinaisons[i]),std::get<1>(combinaisons[i]),std::get<2>(combinaisons[i]));
                    end = clock();
                    localminimaux.push_back(tsp.getCurrentSolutionCost());
                    calculate_time.push_back(end-start);
                    heuristic_type.push_back("randomizedGreedy");
                    nbiterations.push_back(tsp.getNbIterLS()); // nb iterations
                break;
            
            default:
                break;
            }
        }
    }

    vector<tuple<int,int,int>> voisine;
    for(int i=0;i<3;i++){
        voisine.insert(voisine.end(),combinaisons.begin(),combinaisons.end());
    }



    cout<< "Heuristique_type"<<"\t"<< "Voisine_combinaison"<<"\t" << "Nb_iterations"<<"\t" << "Calcul_temp(ms)"<<"\t"<<"local minimal\n"<<endl;
 
    for(int row=1;row<=heuristic_type.size();row++){
        for(int col =0; col<=4;col++){
            switch (col)
            {
                case 0:
                    if(heuristic_type[row-1] == "greedyInit"){
                        cout<<heuristic_type[row-1]<<"      "<<"\t";
                    }else{
                        cout<<heuristic_type[row-1]<<"\t";
                    }
                    break;
                case 1:
                    cout<< "("<<std::get<0>(voisine[row-1])<<","<<
                    std::get<1>(voisine[row-1])<<","<<
                    std::get<2>(voisine[row-1])<<")"<<"\t\t\t\t";
                    break;
                case 2:
                    cout<<nbiterations[row-1]<<"\t\t";
                    break;
                case 3:
                    cout<<calculate_time[row-1]<<"\t\t";
                    break;
                case 4:
                    cout<<localminimaux[row-1]<<"\n";
                    break;
                default:
                    break;
             }

            }

        }

    return 0;

}

