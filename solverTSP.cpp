/********** instanceTSP.cpp **********/

#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <random>
#include <map>
#include "solverTSP.hpp"

using namespace std;



/* Reads instance data. */
void SolverTSP ::readInstanceFromFile(const string& fileName) {
	ifstream infile;
	infile.exceptions(ifstream::failbit | ifstream::badbit);
	infile.open(fileName.c_str());

	// The input files follow the TSPLib "explicit" format.
	string str;
	char * pch;
	char* line;

	while (true) {
		getline(infile, str);
		line = strdup(str.c_str());
		pch = strtok(line, " :");
		if  (strcmp(pch, "DIMENSION") == 0){
			getline(infile, str);
			line = strdup(str.c_str());
			pch = strtok(NULL, " :");
			nbCities = atoi(pch);
		} else if (strcmp(pch, "EDGE_WEIGHT_SECTION") == 0){
			break;
		}
	}

	// Distances from i to j
	distanceMatrix.resize(nbCities*nbCities);
	for(int i = 0; i < nbCities; i++) {
		for (int j = 0; j < nbCities; j++) {
			infile >> distanceMatrix[nbCities * i+j];
		}
	}

	currentSolutionCost=-1;
	bestSolutionCost=-1;

	nbIterLS=0;

}

bool SolverTSP::checkFeasibility() {
	vector<int> copyCurrentSol = curSol;
	copyCurrentSol.pop_back();
	std::sort (copyCurrentSol.begin(), copyCurrentSol.end());
	bool b=true;
	int it=0;
	while (b && it< nbCities){
		b = (copyCurrentSol[it]==it);
		// cout<<copyCurrentSol[it]<<endl;
		it++;
	}
	// cout<<endl;
	if (!b) cout << "Warning: infeasible solution found" << endl;
	return b;
}

void SolverTSP::computeDistanceCost() {
	currentSolutionCost = distance(curSol[nbCities-1], curSol[0]);
	for(int i=0; i < nbCities-1; i++) currentSolutionCost += distance(curSol[i],curSol[i+1]);
}

void SolverTSP::computeReversedDistanceCost() {
	revSolutionCost = 0;
	for(int i = 0; i < revSol.size() - 1; i++) revSolutionCost += distance(revSol[i],revSol[i+1]);
}

void SolverTSP::reverseCities(const int& c1, const int& c2) {
	vector<int>::iterator it1, it2;
    it1 = revSol.begin() + c1;
    it2 = revSol.begin() + c2;
    reverse(it1, it2);
	computeReversedDistanceCost();
}

bool SolverTSP::hillClimbingIter(bool swapMoves, bool revMoves, bool insertMoves) {


	int modeImpr = -1 ; // pour dire quel voisinage a ameliore
	int param1 = -1;
	int param2 = -1;
	int i1, i2;
	int delta = 0;
	int temp = 0;

	if (swapMoves){
		temp = distSol(nbCities-2,nbCities-1) + distSol(nbCities-1,0) +  distSol(0,1) 
					   	- distSol(nbCities-2,0) - distSol(0,nbCities-1) - distSol(nbCities-1,1);
		if (delta < temp ){
			delta=temp;
			param1 = 0;
			param2 = nbCities-1;
			modeImpr=1;
		}

		for(i1=1; i1 < nbCities-1 ; i1++){
			i2=i1+1;
			temp = distSol(i1-1,i1)  +  distSol(i1,i2)  +  distSol(i2,i2+1) - distSol(i1-1,i2) - distSol(i2,i1) -  distSol(i1,i2+1) ;
			if (delta < temp ){
				delta=temp;
				param1 = i1;
				param2 = i2;
				modeImpr=1;
			}
		}
		for(i1=1; i1 < nbCities-1 ; i1++){
			for(i2=i1+2; i2 < nbCities ; i2++){
				temp = distSol(i1-1,i1) + distSol(i1,i1+1) +  distSol(i2-1,i2)+ distSol(i2,i2+1) - distSol(i1-1,i2) - distSol(i2,i1+1) -  distSol(i1,i2+1) -  distSol(i2-1,i1);
				if (delta < temp ){
					delta=temp;
					param1 = i1;
					param2 = i2;
					modeImpr=1;
				}
			}
		}
		i1=0;
		for(i2=2; i2 < nbCities-1 ; i2++){
			temp = distSol(nbCities-1,i1) + distSol(i1,i1+1) +  distSol(i2-1,i2)+  distSol(i2,i2+1) - distSol(nbCities-1,i2) - distSol(i2,i1+1) -  distSol(i1,i2+1) -  distSol(i2-1,i1);
			if (delta < temp ){
				delta=temp;
				param1 = i1;
				param2 = i2;
				modeImpr=1;
			}
		}
	}
	if (revMoves){
		// i2 == i1 + 1 same as insert
		// i2 == i1 + 2 same as swap
		// i2 == i1 + 3 ...
		i1 = 0;
		for (i2 = i1 + 3; i2 < nbCities - 1; i2 ++) {

		}
	}
	if (insertMoves){
		// insert last to first or first to last does not change anything
		// insert to i1 + 1 same as swap

		// insert to i1 + 2...
		for(i1=1; i1 < nbCities-1 ; i1++) {
			for(i2=i1+2; i2 < nbCities ; i2++) {
				temp = distSol(i1-1, i1) + distSol(i1, i1+1) + distSol(i2, i2 + 1) - distSol(i2, i1) - distSol(i1, i2 + 1) - distSol(i1-1, i1+1);
				if (delta < temp ){
					cout<<temp<<' '<<i1<<' '<<i2<<" |";
					delta=temp;
					param1 = i1;
					param2 = i2;
					modeImpr = 3;
				}
			}
		}
		i1 = 0;
		for(i2 = 2; i2 < nbCities-1; i2++){
			temp = distSol(i1, i1+1) + distSol(nbCities-1, nbCities) + distSol(i2, i2 + 1) - distSol(i2, i1) - distSol(i1, i2+1) - distSol(nbCities-1, 1);
			if (delta < temp) {
				// cout<<temp<<' '<<i1<<' '<<i2<<" |";
				delta=temp;
				param1 = i1;
				param2 = i2;
				modeImpr = 3;
			}
		}

		for(i1 = nbCities - 1; i1 > 1 ; i1--) {
			for(i2 = i1 - 2; i2 > 0 ; i2--) {
				temp = distSol(i1-1, i1) + distSol(i1, i1+1) + distSol(i2 - 1, i2) - distSol(i2-1, i1) - distSol(i2, i1) - distSol(i1-1, i1+1);
				if (delta < temp ){
					cout<<temp<<' '<<i1<<' '<<i2<<" |";
					delta=temp;
					param1 = i1;
					param2 = i2;
					modeImpr = 3;
				}
			}
		}

	}

	if (verboseMode) {
		cout << "best move : " << modeImpr << " param1 : " << param1 << " param2 : " << param2 << endl;
	}


	currentSolutionCost -= delta;
	if (modeImpr==1){
		temp = curSol[param2];
		curSol[param2]=curSol[param1];
		curSol[param1]=temp;
		if (param1==0) curSol[nbCities]=temp;
	}

	if (modeImpr==2){
		
	}

	if (modeImpr==3){
		if (param1 > param2) {
			//cout<<"-------------------------"<<endl;
			//for(auto& e: curSol) cout<<e<<' ';
			//cout<<endl<<"insert: ";
			curSol.insert(curSol.begin() + param2, curSol[param1]);
			//for(auto& e: curSol) cout<<e<<' ';
			//cout<<endl<<"erase: ";
			curSol.erase(curSol.begin() + param1 + 1);
			//for(auto& e: curSol) cout<<e<<' ';
			//cout<<endl;
		}
		else {
			//cout<<"-------------------------"<<endl;
			//for(auto& e: curSol) cout<<e<<' ';
			//cout<<endl<<"insert: ";
			curSol.insert(curSol.begin() + param2 + 1, curSol[param1]);
			//for(auto& e: curSol) cout<<e<<' ';
			//cout<<endl<<"erase: ";
			curSol.erase(curSol.begin() + param1);
			//for(auto& e: curSol) cout<<e<<' ';
			//cout<<endl;
		}
		curSol[nbCities] = curSol[0];
	}

	if (verboseMode) printStatus();


	return delta>0;
}

void SolverTSP::hillClimbing(bool swapMoves, bool revMoves, bool insertMoves) {

	nbIterLS=0;
	bool improving = true;
	while(improving && nbIterLS< nbMaxIterLS){
		improving = hillClimbingIter(swapMoves, revMoves, insertMoves);
		nbIterLS++;
	}

}

void SolverTSP::naiveInit() {
	curSol.resize(nbCities+1);
	for(int i=0; i < nbCities; i++) curSol[i]=i;
	curSol[nbCities]=0;
	computeDistanceCost();
}

void SolverTSP::greedyInit() {
	curSol.resize(nbCities+1);
	curSol[0] = 0;
	int cur_index = 0;
	int cur_dist;
	int min_dist;
	for (int i = 1; i < nbCities; i++) {
		min_dist = INT_MAX;
		// find nearest city
		for (int j = 0; j < nbCities; j++) {
			// check if city already in vector
			if (find(curSol.begin(), curSol.begin() + i, j) != curSol.begin() + i) continue;
			cur_dist = distance(curSol[i-1], j);
			if (min_dist > cur_dist) {
				cur_index = j;
				min_dist = cur_dist;
			}
		curSol[i] = cur_index;
		}
	}
	curSol[nbCities]=0;
	computeDistanceCost();
	if (checkFeasibility())
		printStatus();
}

void SolverTSP::randomizedConstr() {
	curSol.resize(nbCities+1);
	random_device rd;
    mt19937 g(rd());
	for (int i = 0; i < nbMaxRestart + 1; i++) {
		curSol.clear();
		curSol.resize(nbCities+1);
		for(int i=0; i < nbCities; i++) curSol[i]=i;
		shuffle(begin(curSol), end(curSol) - 1, g);
		curSol[nbCities] = curSol[0];
		computeDistanceCost();
		updateBestSolution();
		// printStatus();
	}
	curSol = bestSolution;
	currentSolutionCost = bestSolutionCost;
	// if (checkFeasibility())
		//printStatus();
}

void SolverTSP::randomizedGreedy() {
	const int n_cities = 3;
	random_device rd;
	mt19937 g(rd());
	uniform_int_distribution<> dis(0, n_cities - 1);
	vector< pair<int, int> > vec_index_dist;
	
	int cur_index = 0;
	int cur_dist;
	int min_dist;
	int rand_bound;
	for (int k = 0; k < nbMaxRestart + 1; k++) {
		curSol.clear();
		curSol.resize(nbCities+1);
		curSol[0] = 0;
		for (int i = 1; i < nbCities; i++) {
		min_dist = INT_MAX;
		// find nearest city
		for (int j = 0; j < nbCities; j++) {
			// check if city already in vector
			if (find(curSol.begin(), curSol.end(), j) != curSol.end()) continue;
			vec_index_dist.push_back(make_pair(j, distance(curSol[i-1], j)));
		}
		sort(vec_index_dist.begin(), vec_index_dist.end(), [](auto &left, auto &right) { return left.second < right.second; });
		// for(auto& e: vec_index_dist) cout<<e.first<<'-'<<e.second<<' ';
		// cout<<vec_index_dist.size()<<endl;
		size_t rand_index = vec_index_dist.size() < n_cities ? 0 : dis(g);
		curSol[i] = vec_index_dist[rand_index].first;
		vec_index_dist.clear();
		}
		curSol[nbCities]=0;
		computeDistanceCost();
		// printStatus();
		updateBestSolution();
	}
	curSol = bestSolution;
	currentSolutionCost = bestSolutionCost;
	computeDistanceCost();
	if (checkFeasibility())
		printStatus();
}

void SolverTSP::graspHC(bool swapMoves, bool revMoves, bool insertMoves) {
	//TODO

}

void SolverTSP::updateBestSolution() {
	if (bestSolutionCost<0 || bestSolutionCost > currentSolutionCost){
		//if (checkFeasibility()){
			bestSolution = curSol;
			bestSolutionCost=currentSolutionCost;
		//}
	}
}

void SolverTSP::printStatus() {
	cout << "current solution with cost " << currentSolutionCost << endl;

	for(int i=0; i <nbCities; i++){
		cout << " " << curSol[i];
	}
	cout << endl;

}

void SolverTSP ::displayInstance() {
	cout << "nb of cities : " << nbCities << endl;
	cout << "distance matrix : " << endl;

	for(int i1=0; i1 < nbCities; i1++){
		for(int i2=0; i2 < nbCities; i2++) cout << " " << distance(i1,i2);
		cout << endl;
	}
}

void SolverTSP::simulatedAnnealing() {
	//TODO
}




