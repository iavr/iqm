#include "br_kmeans.h"
#include <fstream>
#include <chrono>
#include <sstream>
#include <iostream>
#include <string>
#include "vector_io.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	string inputFile;
	string initialCentersFile;
	dmatrix initialCenters;
	string centerOutputFile = "computed_centers.bin";
	string assignmentsFile = "assignments.bin";
	string outFile;
	unsigned long k = 0;
	int t = 0;
	int maxIter = 20;
	bool verbose = false;

	if (argc<3) {
		cout << "Error. Please specify at least input file and k" << endl;
		return 1;
	}

	inputFile = string(argv[1]);
	istringstream parseK(argv[2]);
	parseK >> k;

	if (argc>=4) {
		istringstream parseT(argv[3]);
		parseT >> t;
	}

	if (argc>=5) {
		istringstream parseMaxIter(argv[4]);
		parseMaxIter >> maxIter;
	}

	if (argc>=6) {
		
		initialCentersFile = string(argv[5]);
		string line;
		ifstream in(initialCentersFile.c_str());
		unsigned long current = 0;
		while (getline(in, line)) {
			initialCenters.push_back(dvector());	
			istringstream iss(line);
			string line2;
			while (getline(iss, line2, ',')) {
				istringstream convert(line2);
				float value; convert >> value;	
				initialCenters[current].push_back(value);
			}
			current++;
		}
	}
		
	Kmeans kmeans(inputFile, k, t, maxIter, verbose);
	if (t==-1) {
		kmeans.l = kmeans.numOfVecs/ k;
	}
	if (argc>=6) {
		kmeans.initializeExplicitly(initialCenters);
	}
	else {
		kmeans.initializeRandomly();
	}
	
	kmeans.runAlgorithm();

	//save_double_array<vector, vector, float, allocator<float>, allocator<vector<float> > >(kmeans.centers, centerOutputFile);
//	save_double_array<vector, vector, float, allocator<float>, allocator<vector<float> > >(kmeans.centers, centerOutputFile);
//	save_array<vector, unsigned, allocator<unsigned> >(kmeans.labels, assignmentsFile);
//	ofstream out("assignments.csv");
//	out << kmeans.labels[0];
//	for (unsigned i=1; i<kmeans.numOfVecs; i++) {
//		out << ", " << kmeans.labels[i];
//	}
//	out.close();

	ofstream outCenters("centers.csv");
	for (unsigned i=0; i<kmeans.k; i++) {
		outCenters << kmeans.centers[i][0];
		for (int j=1; j<kmeans.dimension; j++) {
			outCenters << ", " << kmeans.centers[i][j];
		}
		outCenters << "\n";
	}
	outCenters.close();

}
