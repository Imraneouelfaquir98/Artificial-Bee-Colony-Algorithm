#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include <conio.h>
#include <time.h>

using namespace std;

#define FoodNumber 20
#define limit 100
#define maxCycle 3000
#define D 50
#define lb -5.12
#define ub  5.12

double Foods[FoodNumber][D];
double f[FoodNumber];
double fitness[FoodNumber];
double trial[FoodNumber];
double prob[FoodNumber];
double solution[D];
double ObjValSol;
double FitnessSol ;
double GlobalMin;
double GlobalParams [D];
double r;
int neighbour, param2change;

double sphere(double sol[D]){
	int j;
	double top = 0;
	for (j = 0; j<D; j++)
		top = top + sol[j] * sol[j];
	return top;
}


double CalculateFitness(double fun){
	double result = 0;
	if (fun >= 0)
		result = 1 / (fun + 1);
	else
		result = 1 + fabs(fun);
	return result;
}


void MemorizeBestSource(){
	int i, j ;

	for (i = 0; i<FoodNumber; i++)
		if (f [i]<GlobalMin)
		{
			GlobalMin = f [i];
			for (j = 0; j < D; j++)
				GlobalParams [j] = Foods [i][j];
		}
}

void init(int index){
	int j;
	for (j = 0; j<D; j++){
		r = ((double)rand () / ((double)(RAND_MAX)+(double)(1)));
		Foods[index][j] = r*(ub - lb) + lb;
		solution[j] = Foods[index][j];
	}
	f[index] = sphere(solution);
	fitness[index] = CalculateFitness(f[index]);
	trial[index] = 0;
}

void initial(){
	int i;
	for (i = 0; i<FoodNumber; i++)
		init(i);

	GlobalMin = f [0];
	for (i = 0;i < D; i++)
		GlobalParams [i] = Foods [0][i];
}

void SendEmployedBees(){
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
		ObjValSol = sphere(solution);
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


void CalculateProbabilities(){
	int i;
	double maxfit;
	maxfit = fitness[0];
	for (i = 1; i<FoodNumber; i++)
		if (fitness[i]>maxfit)
			maxfit = fitness[i];
	for (i = 0; i<FoodNumber; i++)
		prob[i] = (0.9*(fitness[i] / maxfit)) + 0.1;
}

void SendOnlookerBees(){
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
			ObjValSol = sphere(solution);
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


void SendScoutBees(){
	int maxtrialindex, i;
	maxtrialindex = 0;
	for (i = 1; i<FoodNumber; i++)
		if (trial[i]>trial[maxtrialindex])
			maxtrialindex = i;
	if (trial[maxtrialindex] >= limit)
		init(maxtrialindex);
}


int main(int argc, char const *argv[])
{
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
	return 0;
}