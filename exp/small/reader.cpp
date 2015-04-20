#include "reader.h"
#include "vector_io.hpp"

using namespace std;

void Reader::readFvecs(string filename, vector<vector<float> >& features) {
	ifstream in(filename.c_str(), ios::binary);

	int dimension;
	in.read((char* ) &dimension, sizeof(int));

	in.seekg(0, in.end);
	int numOfVecs = in.tellg();
	numOfVecs = numOfVecs / ((dimension+1)*sizeof(float));

	features.reserve(numOfVecs);
	in.seekg(0, in.beg);
	int discardValue;
	for (int i=0; i<numOfVecs; i++) {
		in.read((char* ) &discardValue, sizeof(int));

		features.push_back(vector<float>(dimension));
		in.read((char*) &(features[i][0]), dimension*sizeof(float));
	}
	in.close();
}

void Reader::readVector(std::string filename, std::vector<std::vector<float> >& features) {
	load_double_array<float>(filename, features);
}
