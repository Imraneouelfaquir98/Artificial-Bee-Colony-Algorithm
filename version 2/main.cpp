#include <iostream>
#include <vector>
#include "abc.h"

using namespace std;

double  sigmoid    (vector<double> Xi, vector<double> W)
int     predict    (vector<double> X , vector<double> W);

void    load_data  (
		vector<vector<double>> *X_train, 
		vector<vector<double>> *X_test, 
		vector<int> *y_train,
		vector<int> *y_test
	);
double cost    (
		vector<vector<double>> X, 
		vector<int> y, 
		vector<double> W
	);
double accuracy(
		vector<vector<double>> X_test, 
		vector<int> y_test, 
		vector<double> W
	);

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

double sigmoid(vector<double> Xi, vector<double> W){
	double val = 0;
	for (int i = 0; i < (int)Xi.size(); ++i)
		val += (double) Xi[i] * W[i];
	return 1/(1 + exp(-val));
}

double cost(vector<vector<double>> X, vector<double> y, vector<double> W){
	double val = 0;
	for(int i=0; i<(int)X.size(); i++) 
		val += pow(y[i] - sigmoid(X[i], W), 2);
	return (double)val/X.size();
}

double object_func(vector<double> sol){
	int j;
	double top = 0;
	for (j = 0; j<(int)sol.size(); j++)
		top = top + sol[j] * sol[j];
	return top;
}

void load_data(
		vector<vector<double>> *X_train, 
		vector<vector<double>> *X_test, 
		vector<int> *y_train,
		vector<int> *y_test
	){
	cv::Mat image;
	string path = "./PNEUMONIA/";
	for (int k = 0; k<2; k++){
		for (int i = 0; i < 80; ++i){
			image = cv::imread(path+to_string(k)+"_"+to_string(i+1)+".png",cv::IMREAD_GRAYSCALE);
			cv::resize(image, image, cv::Size(100, 100), 0, 0, cv::INTER_AREA);
			if(i%5 == 0){
				X_test->push_back(image_to_vector(image));
				y_test->push_back(k);
			}
			else{
				X_train->push_back(image_to_vector(image));
				y_train->push_back(k);
			}
			// cv::imshow("Image", image);
			// cv::waitKey(0);
		}
		path = "./NORMAL/";
	}
}

int predict(vector<double> X, vector<double> W){
	if(sigmoid(X, W) >= 0.5)
		return 1;
	else
		return 0;
}

double accuracy(vector<vector<double>> X_test, vector<int> y_test, vector<double> W){
	double acc = 0;
	for (int i = 0; i < (int)X_test.size(); ++i)
		acc += (predict(X_test[i], W) == y_test[i])? 1.0 : 0.0;
	return acc/(double)X_test.size();
}
