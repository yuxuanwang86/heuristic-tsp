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
		it++;
		b= (copyCurrentSol[it]==it);
	}
	if (!b) cout << "Warning: infeasible solution found" << endl;
	return b;
}

void SolverTSP::computeDistanceCost() {
	currentSolutionCost=distance(curSol[nbCities-1],curSol[0]);
	for(int i=0; i < nbCities-1; i++) currentSolutionCost+=distance(curSol[i],curSol[i+1]);
}




bool SolverTSP::hillClimbingIter(bool swapMoves, bool revMoves, bool insertMoves) {


	int modeImpr = -1 ; // pour dire quel voisinage a ameliore
	int param1 = -1;
	int param2 = -1;
	int i1, i2;
	int delta = 0;
	int temp = 0;

	if (swapMoves){

/*
		temp = distance(curSol[nbCities-2],curSol[nbCities-1]) + distance(curSol[nbCities-1],curSol[0]) +  distance(curSol[0],curSol[1]) - distance(curSol[nbCities-2],curSol[0])  -  distance(curSol[0],curSol[nbCities-1]) -  distance(curSol[nbCities-1],curSol[1]);
		if (delta < temp ){
			delta=temp;
			param1 = 0;
			param2 = nbCities-1;
			modeImpr=1;
		}

		for(i1=1; i1 < nbCities-2 ; i1++){
			i2=i1+1;
			temp = distance(curSol[i1-1],curSol[i1])  +  distance(curSol[i1],curSol[i2])  +  distance(curSol[i2],curSol[i2+1]) - distance(curSol[i1-1],curSol[i2]) -  distance(curSol[i1],curSol[i2+1]) -  distance(curSol[i2],curSol[i1]) ;
			if (delta < temp ){
				delta=temp;
				param1 = i1;
				param2 = i2;
				modeImpr=1;

			}
		}

		for(i1=1; i1 < nbCities-1 ; i1++){
			for(i2=i1+2; i2 < nbCities ; i2++){
				temp = distance(curSol[i1-1],curSol[i1]) + distance(curSol[i1],curSol[i1+1]) +  distance(curSol[i2-1],curSol[i2])+  distance(curSol[i2],curSol[i2+1]) - distance(curSol[i1-1],curSol[i2]) - distance(curSol[i2],curSol[i1+1]) -  distance(curSol[i1],curSol[i2+1]) -  distance(curSol[i2-1],curSol[i1]);
				if (delta < temp ){
					delta=temp;
					param1 = i1;
					param2 = i2;
					modeImpr=1;
				}

			}
		}*/


		temp = distSol(nbCities-2,nbCities-1) + distSol(nbCities-1,0) +  distSol(0,1) - distSol(nbCities-2,0) - distSol(0,nbCities-1) - distSol(nbCities-1,1);
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
				temp = distSol(i1-1,i1) + distSol(i1,i1+1) +  distSol(i2-1,i2)+  distSol(i2,i2+1) - distSol(i1-1,i2) - distSol(i2,i1+1) -  distSol(i1,i2+1) -  distSol(i2-1,i1);
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
		//TODO
	}
	if (insertMoves){
		//TODO
	}

	if (verboseMode) {
		cout << "best move : " << modeImpr << " param1 : " << param1 << " param2 : " << param2 << endl;
	}


	currentSolutionCost-=delta;
	if (modeImpr==1){
		temp = curSol[param2];
		curSol[param2]=curSol[param1];
		curSol[param1]=temp;
		if (param1==0) curSol[nbCities]=temp;
	}

	if (modeImpr==2){
		//TODO
	}

	if (modeImpr==3){
		//TODO
	}

	if (verboseMode) {	printStatus();	}


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
		// TODO do not use distance and find min(distanceMatrix(nbCities*i, nbCities*i+nbCities))
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
		curSol[nbCities]=0;
		computeDistanceCost();
		updateBestSolution();
		//printStatus();
	}
	curSol = bestSolution;
	currentSolutionCost = bestSolutionCost;
	printStatus();
}

void SolverTSP::randomizedGreedy() {
	const int n_cities = 3;
	random_device rd;
	mt19937 g(rd());
	uniform_int_distribution<> dis(0, n_cities-1);
	vector< pair<int, int> > vec_index_dist;
	
	int cur_index = 0;
	int cur_dist;
	int min_dist;
	for (int k = 0; k < nbMaxRestart + 1; k++) {
		curSol.clear();
		curSol.resize(nbCities+1);
		curSol[0] = 0;
		for (int i = 1; i < nbCities; i++) {
		min_dist = INT_MAX;
		// TODO do not use distance and find min(distanceMatrix(nbCities*i, nbCities*i+nbCities))
		// find nearest city
		for (int j = 0; j < nbCities; j++) {
			// check if city already in vector
			if (find(curSol.begin(), curSol.end(), j) != curSol.end()) continue;
			vec_index_dist.push_back(make_pair(j, distance(curSol[i-1], j)));
		}
		sort(vec_index_dist.begin(), vec_index_dist.end(), [](auto &left, auto &right) { return left.second < right.second; });
		// for(auto& e: vec_index_dist) cout<<e.first<<'-'<<e.second<<' ';
		// cout<<vec_index_dist.size()<<endl;
		size_t rand_index = dis(g);
		curSol[i] = vec_index_dist[min(vec_index_dist.size()-1, rand_index)].first;
		vec_index_dist.clear();
		}
		updateBestSolution();
	}
	curSol[nbCities]=0;
	curSol = bestSolution;
	currentSolutionCost = bestSolutionCost;
	computeDistanceCost();
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

	for(int i=0; i < nbCities; i++){
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




