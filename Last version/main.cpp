#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "abc.h"

using namespace std;

double  sigmoid    (vector<double> Xi, vector<double> W);
int     predict    (vector<double> X , vector<double> W);
vector<double> image_to_vector(cv::Mat image);

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
	vector<vector<double>> X_train, X_test;
	vector<int> y_train, y_test;

	cout<<"Loiding data"<<endl;
	load_data(&X_train, &X_test, &y_train, &y_test);
	cout<<"Data loaded"<<endl;

	vector<double> Cmin (X_train[0].size(),-20), Cmax (X_train[0].size(), 20);

	ABC abc = ABC(
        X_train[0].size(), // D = Dimension
		  20,              // FoodNumber
		 100,              // limit
		 100,              // maxCycle
		Cmin,              // Cmin
		Cmax,              // Cmax
		&cost       	   // Objective function
	);
	abc.fit(X_train, y_train);

	cout<<"\nTesting dataset:"<<endl;
	for (int i = 0; i < (int)X_test.size(); ++i)
		cout<<"  y_true = "
				<<y_test[i]
				<<", y_pred = "
				<<predict(X_test[i], abc.solution)
				<<", sigmoid = "
				<<sigmoid(X_test[i], abc.solution)<<endl;

	cout<<"\nAccuracy = "
			<<accuracy(X_test, y_test, abc.solution)*100
			<<" %\n"
			<<endl;

	return 0;
}

double sigmoid(vector<double> Xi, vector<double> W){
	double val = 0;
	for (int i = 0; i < (int)Xi.size(); ++i)
		val += (double) Xi[i] * W[i];
	return 1/(1 + exp(-val));
}

double cost(vector<vector<double>> X, vector<int> y, vector<double> W){
	double val = 0;
	for(int i=0; i<(int)X.size(); i++) 
		val += pow(y[i] - sigmoid(X[i], W), 2);
	return (double)val/X.size();
}

vector<double> image_to_vector(cv::Mat image){
  vector<double> x;
  x.push_back(1);
  for (int i = 0; i < image.rows; ++i)
  	for (int j = 0; j < image.cols; ++j)
  		x.push_back((double)image.at<uchar>(i, j)/255);
  return x;
}

void load_data(
		vector<vector<double>> *X_train, 
		vector<vector<double>> *X_test, 
		vector<int> *y_train,
		vector<int> *y_test
	){
	cv::Mat image;
	string path = "./with_mask/";
	for (int k = 0; k<2; k++){
		for (int i = 0; i < 40; ++i){
			image = cv::imread(path+to_string(k)+"_"+to_string(i)+".jpg",cv::IMREAD_GRAYSCALE);
			cv::resize(image, image, cv::Size(80, 80), 0, 0, cv::INTER_AREA);
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
		path = "./without_mask/";
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


// g++ -c main.cpp abc.cpp -std=c++11 `pkg-config --cflags --libs opencv`
// g++ main.o abc.o -o output -std=c++11 `pkg-config --cflags --libs opencv`
// ./output