#ifndef __KMEANS_H__
#define __KMEANS_H__

#include "pla.h"
#include "reader.h"
#include <flann/flann.hpp>

class Kmeans {
public:
	Kmeans(std::string dataFilename, unsigned long k, int l, int maxIter=100, bool verbose=false);
	void runAlgorithm();
	void initializeRandomly();
	void initializeExplicitly(dmatrix& initialCenters);
	void assignStep(flann::Index<flann::L2<float> >& index, dmatrix& centers);
	void assignStep();
	void postInitialization();
	double computeScore(dmatrix& centers);
	double silhouette();

	dmatrix dataset;
	dmatrix centers;
	uvector labels; 
	unsigned long numOfVecs;
	int dimension;
	umatrix assignments;
	unsigned long k;
	int l;
	int maxIter;
	bool verbose;
	dvector distances;
};

#endif
