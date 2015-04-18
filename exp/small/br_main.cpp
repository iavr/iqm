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
		
	Kmeans kmeans(inputFile, k, t, maxIter, verbose);
	if (t==-1) {
		kmeans.l = kmeans.numOfVecs/ k;
	}
	kmeans.initializeRandomly();
	kmeans.runAlgorithm();

	save_double_array<vector, vector, float, allocator<float>, allocator<vector<float> > >(kmeans.centers, centerOutputFile);
	save_array<vector, unsigned, allocator<unsigned> >(kmeans.labels, assignmentsFile);
	ofstream out("assignments.csv");
	out << kmeans.labels[0];
	for (unsigned i=1; i<kmeans.numOfVecs; i++) {
		out << ", " << kmeans.labels[i];
	}
	out.close();
}
