# Artificial-Bee-Colony-Algorithm
This project aims to implement the Artificial Bee Colony algorithm with the programming language C ++ and to apply in mask detection using C++ version of OpenCV.

The implementation of the algorithm will be in the abc.cpp and abc.h files and the sample application will be in the main.cpp file.

The command lines to run this code on Linux system are as follows:

g++ -c main.cpp abc.cpp -std=c++11 `pkg-config --cflags --libs opencv`
g++ main.o abc.o -o output -std=c++11 `pkg-config --cflags --libs opencv`
./output
