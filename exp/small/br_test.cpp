#include "br_kmeans.h"
#include <ivl/ivl>
#include <chrono>
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
	string filename = "/home/user/sift/raw/sift_base.fvecs";
	string centroidsFilename = "./initialCentroids.csv";
	unsigned long ks[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
	int maxIter = 20;
	bool verbose = false;

	ofstream out("assignments.csv");

	for (int i=9; i<10; i++) {
		int l = 100000 / ks[i];
		Kmeans kmeans(filename, ks[i], l, maxIter, verbose);
	
	//
	//	dmatrix initialCenters(k);
	//	for (unsigned long i=0; i<k; i++) {
	//		initialCenters.push_back(dvector());
	//	}
	//	readInitialCenters(centroidsFilename, initialCenters);
	
		chrono::high_resolution_clock::time_point t1, t2; 
		t1 = chrono::high_resolution_clock::now();
		kmeans.initializeRandomly();
		t2 = chrono::high_resolution_clock::now();
		chrono::duration<double> initializationTime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
		t1 = chrono::high_resolution_clock::now();
		kmeans.runAlgorithm();
		t2 = chrono::high_resolution_clock::now();
		chrono::duration<double> loopTime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
//		kmeans.assignStep();
//		double score = kmeans.computeScore(kmeans.centers);
//		cout << score << endl;
		cout << "Total time" << (initializationTime.count()+loopTime.count()) << ", loop time: " << loopTime.count() << ", init time: " << initializationTime.count() << endl;

//		out << kmeans.labels[0];
//		for (unsigned long i=1; i<100000; i++) {
//			out << ", " << kmeans.labels[i];	
//		}
//		out << "\n";
	}
	out.close();
}
