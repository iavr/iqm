#include "kmeans.h"
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

void readInitialCenters(string filename, dmatrix& initialCenters) {
	ifstream in(filename.c_str());
	string line;
	int i=0;
	while (getline(in, line)) {
		if (line.empty()) continue;
		istringstream iss(line);	
		string strVal;
		int j=0;
		while (getline(iss, strVal, ',')) {
			istringstream conv(strVal);
			float val; conv >> val;
			initialCenters[i].push_back(val);
			j++;
		}
		i++;
	}
	in.close();
}

int main() {
	string filename = "/home/user/sift/raw/sift_learn.fvecs";
	string centroidsFilename = "./initialCentroids.csv";
	unsigned long k = 1000;
	int maxIter = 10;
	bool verbose = true;
	Kmeans kmeans(filename, k, maxIter, verbose);
	
	ofstream out("../../sift/raw/sift_learn.csv");
	for (unsigned long i=0; i<kmeans.numOfVecs; i++) {
		out << kmeans.dataset[i][0];
		for (int dim =1; dim<kmeans.dimension; dim++) {
			out << ", " << kmeans.dataset[i][dim];
		}
		out << "\n";
	}
	out.close();

//	dmatrix initialCenters(k);
//	for (unsigned long i=0; i<k; i++) {
//		initialCenters.push_back(dvector());
//	}
//	readInitialCenters(centroidsFilename, initialCenters);
//
//	kmeans.initializeExplicitly(initialCenters);
//	kmeans.runAlgorithm();
}
