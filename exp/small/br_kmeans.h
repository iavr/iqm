#ifndef __KMEANS_H__
#define __KMEANS_H__

#include "pla.h"
#include "reader.h"
#include <flann/flann.hpp>

class Kmeans {
public:
	Kmeans(std::string dataFilename);
	void runAlgorithm(unsigned long k, int l, int maxIter=100, bool verbose=false);
	virtual void assignStep(flann::Index<flann::L2<float> >& index, dmatrix& centers);
	double computeScore(dmatrix& centers);
	double silhouette(dmatrix& centers);

	dmatrix dataset;
	unsigned long numOfVecs;
	int dimension;
	umatrix assignments;
	unsigned long k;
	int l;
	dvector distances;
	uvector nearestCluster;
};

#endif
