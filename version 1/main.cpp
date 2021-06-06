#include <iostream>
#include <vector>
#include "abc.h"

using namespace std;

double object_func(vector<double> sol);

int main(int argc, char const *argv[])
{

	vector<double> Cmax (50,-5.12), Cmin (50, 5.12);

	ABC abc = ABC(
		  50,  // D
		  20,  // FoodNumber
		 100,  // limit
		3000,  // maxCycle
		Cmin,  // Cmin
		Cmax,
		&object_func
	);
	abc.fit();

	for (int i = 0; i < (int)abc.solution.size(); ++i)
		cout<<i<<"- "<<abc.solution[i]<<endl;

	return 0;
}

double object_func(vector<double> sol){
	int j;
	double top = 0;
	for (j = 0; j<(int)sol.size(); j++)
		top = top + sol[j] * sol[j];
	return top;
}
