#ifndef __KMEANS_H__
#define __KMEANS_H__

#include "pla.h"
#include "reader.h"

class Kmeans {
public:
	Kmeans(std::string dataFilename);
	void runAlgorithm(unsigned long k, int maxIter=100, bool verbose=false);
	virtual void assignStep(dmatrix& centers);
	double computeScore(dmatrix& centers);
	double silhouette(dmatrix& centers);

	dmatrix dataset;
	unsigned long numOfVecs;
	int dimension;
	umatrix assignments;
	unsigned long k;
};

#endif
