#ifndef ABC_H
#define ABC_H

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <bits/stdc++.h>
#include <math.h>
#include <vector>

using namespace std;

class ABC{
public:
	int FoodNumber = 20, limit = 100, maxCycle = 3000, D = 50;
	double lb = -5.12, ub = 5.12;

	vector<vector<double>> Foods;
	vector<double> f;
	vector<double> fitness;
	vector<double> trial;
	vector<double> prob;
	vector<double> solution;
	vector<double> GlobalParams;
	vector<double> Cmin, Cmax;


	double ObjValSol;
	double FitnessSol ;
	double GlobalMin;

	double r;
	int neighbour, param2change;

	ABC(
		int D,
		int FoodNumber, 
		int limit, 
		int maxCycle, 
		vector<double> Cmin,
		vector<double> Cmax,
		double (*func)(vector<double> sol)
		// double (*func)(vector<vector<float>> X, vector<float> y, vector<float> W)
	);
	
	double (*object_func)(vector<double> sol);
	void 	fit();
	// double  object_func             (vector<double> sol);
	double 	CalculateFitness		(double fun);
	void 	MemorizeBestSource		();
	void 	init					(int  index);
	void 	initial					();
	void 	SendEmployedBees		();
	void 	CalculateProbabilities	();
	void 	SendOnlookerBees		();
	void 	SendScoutBees			();
};

#endif // ABC_H