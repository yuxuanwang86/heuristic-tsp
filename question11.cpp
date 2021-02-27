/********** testTSP.cpp **********/

#include "solverTSP.hpp"
#include <iostream>
#include <tuple>
#include<time.h>
#include <fstream>
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

    string filename = argv[1];
    int result = getResult(filename); // get the result/optimum


    SolverTSP tsp;
    tsp.setVerboseMode(true);
	tsp.setLSparameters(20,1);

    tsp.readInstanceFromFile(instanceFile);
    
    vector<string> recherche_methode;
    vector<int> quality;
    vector<double> temps_calcul;
    vector<tuple<int,int,int>> combinaisons;
    vector<tuple<int,int,int>> voisinages;

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

    for(int i=0;i<2;i++){
        voisinages.insert(voisinages.end(),combinaisons.begin(),combinaisons.end());
    }

    for(int x=0;x<2;x++){
        for(int i=0;i<combinaisons.size();i++){
              switch (x)
             {
                 case 0/* constant-expression */:
                    recherche_methode.push_back("graspHC");
                    start = clock();
                    tsp.graspHC(std::get<0>(combinaisons[i]),std::get<1>(combinaisons[i]),std::get<2>(combinaisons[i]));
                    end = clock();
                    quality.push_back(tsp.getCurrentSolutionCost() - result);
                    temps_calcul.push_back(end-start);
                 break;

                 case 1:
                    recherche_methode.push_back("ilsHC");
                    start = clock();
                    tsp.ilsHC(std::get<0>(combinaisons[i]),std::get<1>(combinaisons[i]),std::get<2>(combinaisons[i]));
                    end = clock();
                    quality.push_back(tsp.getCurrentSolutionCost() - result);
                    temps_calcul.push_back(end-start);

                 break;
        
                 default:
                 break;
             }

        }
    }

    std::get<0>(curr_comb) = 0; std::get<1>(curr_comb) = 0; std::get<2>(curr_comb) = 0;
    voisinages.push_back(curr_comb);
    recherche_methode.push_back("simulatedAnnealing");
    start = clock();
    tsp.simulatedAnnealing();
    end = clock();
    quality.push_back(tsp.getCurrentSolutionCost() - result);
    temps_calcul.push_back(end-start);

    //result tabs
    // first method
    cout<<"Recherche_Mehode"<<"\t"<<"Voisinage_Combinaison"<<"\t"<<"Distance"<<"\t"<<"temps_calcal\n"<<endl;
   
	
	for(int i=0;i<voisinages.size();i++){
		for(int j=0;j<4;j++){
			switch (j)
			{
			case 0:
                if(recherche_methode[i]!="simulatedAnnealing"){
				    cout<<recherche_methode[i]<<"\t\t\t\t";
                }else{
                    cout<<recherche_methode[i]<<"\t\t";
                }
				break;
			case 1:
				cout<< "("<<std::get<0>(voisinages[i])<<","<<
					std::get<1>(voisinages[i])<<","<<
					std::get<2>(voisinages[i])<<")"<<"\t\t";
				break;
			case 2:
	
					cout<<quality[i]<<"\t";
				
				break;
			case 3:
				cout<<temps_calcul[i]<<"\n";

			default:
				break;
			}
		}
	}
	

    return 0;

}

