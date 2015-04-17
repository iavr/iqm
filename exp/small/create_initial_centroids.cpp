#include "kmeans.h"
#include <fstream>

using namespace std;

int main() {
	string filename = "/home/user/sift/raw/sift_learn.fvecs";
	unsigned long k = 1000;
	int maxIter = 10;
	bool verbose = true;
	Kmeans kmeans(filename, k, maxIter, verbose);
	kmeans.initializePP();

	ofstream out("./initialCentroids.csv");
	for (unsigned long i=0; i<k; i++) { 
		out << kmeans.centers[i][0];
		for (int dim=1; dim<kmeans.dimension; dim++) {
			out << ", " << kmeans.centers[i][dim];
		}
		out << "\n";
	}
	out.close();
}
