#ifndef __KMEANS_H__
#define __KMEANS_H__

#include "pla.h"
#include "reader.h"

class Kmeans {
public:
	Kmeans(std::string dataFilename, unsigned long k, int maxIter=100, bool verbose=false);
	void runAlgorithm();
	void initializeRandomly();
	void initializeExplicitly(dmatrix& initialCenters);
	void initializePP();
	virtual void assignStep(dmatrix& centers);
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
	int maxIter;
	bool verbose;
};

#endif
