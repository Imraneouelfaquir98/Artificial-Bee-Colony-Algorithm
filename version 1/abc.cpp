#include <iostream>
#include "abc.h"
#include <vector>

using namespace std;

ABC::ABC(
		int D,
		int FoodNumber, 
		int limit, 
		int maxCycle, 
		vector<double> Cmin,
		vector<double> Cmax,
		double (*func)(vector<double> sol)
	){

	this->D          = D;
	this->FoodNumber = FoodNumber;
	this->limit      = limit;
	this->maxCycle   = maxCycle;
	this->Cmin       = Cmin;
	this->Cmax       = Cmax;

	object_func = func;

	Foods       .resize(FoodNumber, vector<double>(D, 0));
	f           .resize(FoodNumber, 0);
	fitness     .resize(FoodNumber, 0);
	trial       .resize(FoodNumber, 0);
	prob        .resize(FoodNumber, 0);
	solution    .resize(D, 0);
	GlobalParams.resize(D, 0);
}

// double ABC::object_func(vector<double> sol){
// 	int j;
// 	double top = 0;
// 	for (j = 0; j<D; j++)
// 		top = top + sol[j] * sol[j];
// 	return top;
// }


double ABC::CalculateFitness(double fun){
	double result = 0;
	if (fun >= 0)
		result = 1 / (fun + 1);
	else
		result = 1 + fabs(fun);
	return result;
}


void ABC::MemorizeBestSource(){
	int i, j ;

	for (i = 0; i<FoodNumber; i++)
		if (f [i]<GlobalMin)
		{
			GlobalMin = f [i];
			for (j = 0; j < D; j++)
				GlobalParams [j] = Foods [i][j];
		}
}

void ABC::init(int index){
	int j;
	for (j = 0; j<D; j++){
		r = ((double)rand () / ((double)(RAND_MAX)+(double)(1)));
		Foods[index][j] = r*(Cmax[j] - Cmin[j]) + Cmin[j];
		solution[j] = Foods[index][j];
	}
	f[index] = object_func(solution);
	fitness[index] = CalculateFitness(f[index]);
	trial[index] = 0;
}

void ABC::initial(){
	int i;
	for (i = 0; i<FoodNumber; i++)
		init(i);

	GlobalMin = f [0];
	for (i = 0;i < D; i++)
		GlobalParams [i] = Foods [0][i];
}

void ABC::SendEmployedBees(){
	int i, j;
	for (i = 0 ;i<FoodNumber ;i++){
		r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
		param2change = (int)(r*D);
		r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
		neighbour = (int)(r*FoodNumber);
		while (neighbour == i){
			r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
			neighbour = (int)(r*FoodNumber);
		}
		for (j = 0; j < D; j++)
			solution[j] = Foods[i][j];
		// v_{ij}=x_{ij}+\phi_{ij}∗(x_{kj}−x_{ij}) 
		r =((double)rand() / ((double)(RAND_MAX)+(double)(1)));
		solution[param2change] = Foods[i][param2change] + (Foods[i][param2change] - Foods[neighbour][param2change])*(r - 0.5) * 2;
		if (solution[param2change]<lb)
			solution[param2change] = lb;
		if (solution[param2change]>ub)
			solution[param2change] = ub;
		ObjValSol = object_func(solution);
		FitnessSol = CalculateFitness(ObjValSol);
		// greedy selection 
		if (FitnessSol>fitness[i]){
			trial[i] = 0;
			for (j = 0; j < D; j++)
				Foods[i][j] = solution[j];
			f[i] = ObjValSol;
			fitness[i] = FitnessSol;
		}
		else
			trial[i] = trial[i] + 1;
	}
}


void ABC::CalculateProbabilities(){
	int i;
	double maxfit;
	maxfit = fitness[0];
	for (i = 1; i<FoodNumber; i++)
		if (fitness[i]>maxfit)
			maxfit = fitness[i];
	for (i = 0; i<FoodNumber; i++)
		prob[i] = (0.9*(fitness[i] / maxfit)) + 0.1;
}

void ABC::SendOnlookerBees(){
	int i, j, t;
	i = 0;
	t = 0;
	while (t<FoodNumber){
		r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
		if (r<prob[i]) // choose a food source depending on its probability to be chosen
		{
			t++;
			r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
			param2change = (int)(r*D);

			r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
			neighbour = (int)(r*FoodNumber);

			while (neighbour == i){
				r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
				neighbour = (int)(r*FoodNumber);
			}
			for (j = 0; j < D; j++)
				solution[j] = Foods[i][j];

			// v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}−x_{ij})
			r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
			solution [param2change] = Foods[i] [param2change] + (Foods[i][param2change] - Foods[neighbour][param2change])*(r - 0.5) * 2;
			if (solution[param2change]<lb)
				solution[param2change] = lb;
			if (solution[param2change]>ub)
				solution[param2change] = ub;
			ObjValSol = object_func(solution);
			FitnessSol = CalculateFitness(ObjValSol);
			// greedy selection
			if (FitnessSol>fitness[i]){
				trial[i] = 0;
				for (j = 0; j < D; j++)
					Foods[i][j]=solution[j];
				f[i] = ObjValSol;
				fitness[i] = FitnessSol;
			}
			else
				trial[i] = trial[i] + 1;
		}
		i++;
		if (i == FoodNumber)
			i = 0 ;
	}// while
}


void ABC::SendScoutBees(){
	int maxtrialindex, i;
	maxtrialindex = 0;
	for (i = 1; i<FoodNumber; i++)
		if (trial[i]>trial[maxtrialindex])
			maxtrialindex = i;
	if (trial[maxtrialindex] >= limit)
		init(maxtrialindex);
}

void ABC::fit(){
	int cycle, j;
	double mean;
	mean = 0;
	srand(time(NULL));
	initial();
	MemorizeBestSource();
	for (cycle = 0; cycle<maxCycle; cycle++){
		SendEmployedBees();
		CalculateProbabilities();
		SendOnlookerBees();
		MemorizeBestSource();
		SendScoutBees();
	}
	for (j = 0; j < D; j++)
		cout << "GlobalParam[" << j + 1 << "]: " << GlobalParams[j] << endl;
	cout << "GlobalMin = " << GlobalMin << endl;
}
