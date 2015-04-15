#include "kmeans.h"
#include <set>
#include <random>
#include <iostream>
#include <chrono>

using namespace std;

Kmeans::Kmeans(string dataFilename) {
	Reader::readFvecs(dataFilename, this->dataset);
	numOfVecs = dataset.size();
	dimension = dataset[0].size();
}

void Kmeans::runAlgorithm(unsigned long k, int maxIter, bool verbose) {
	/** Initialization
	 */
	this->k = k;
	std::uniform_int_distribution<int> distribution(0, numOfVecs-1);
	default_random_engine generator;
	set<int> initialCentersIds;
	dmatrix centers;
	centers.reserve(k);

	assignments.clear();
	assignments.reserve(k);

	int currentIndex = 0;
	while (initialCentersIds.size()<k) {
		int id = distribution(generator);
		if (initialCentersIds.find(id)==initialCentersIds.end()) {
			initialCentersIds.insert(id);
			centers.push_back(vector<float>());
			
			for (int j=0; j<dimension; j++) {
				centers[currentIndex].push_back(dataset[id][j]);
			}
			currentIndex++;
			assignments.push_back(vector<unsigned>());
		}
	}
	initialCentersIds.clear();


	/** Main Loop
	 */
	chrono::high_resolution_clock::time_point t1, t2; 
	for (int iter=0; iter<maxIter; iter++) {
		/** Assign Step
		 */
		t1 = chrono::high_resolution_clock::now();
		assignStep(centers); 
		t2 = chrono::high_resolution_clock::now();
		chrono::duration<double> assignTime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);

		/** Update Step
		 */
		t1 = chrono::high_resolution_clock::now();
		centers.clear();
		for (unsigned cluster=0; cluster<k; cluster++) {
			dvector newCenter(dimension, 0);
			for (unsigned i=0; i<assignments[cluster].size(); i++) {
				PLA::add(newCenter, dataset[assignments[cluster][i]]);
			}
			PLA::multiply(newCenter, 1.0/ assignments[cluster].size());
			centers.push_back(newCenter);
		}
		t2 = chrono::high_resolution_clock::now();
		chrono::duration<double> updateTime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);

		/** Compute Score
		 */
		if (verbose) {
			double score = computeScore(centers);
//			double sil = silhouette(centers);
			cout << "Iteration #" << (iter+1) << ":"  << endl;
			cout << "\tKMeans objective score: " << score << endl;
			//cout << "\tSilhouetter score: " << sil << endl;
			cout <<	"\tTotal time: " << (assignTime.count() + updateTime.count()) << "s, update time: " << updateTime.count() << "s, assign time: " << assignTime.count() << "s" << endl;
		}
	}
}

double Kmeans::computeScore(dmatrix& centers) {
	double score = 0;
	for (unsigned cluster=0; cluster<k; cluster++) {
		for (unsigned i=0; i<assignments[cluster].size(); i++) {
			score += PLA::euclideanDistanceSquared(centers[cluster], dataset[assignments[cluster][i]]);
		}
	}
	return score;
}

double Kmeans::silhouette(dmatrix& centers) {
	double silhouetteScore = 0;
	for (unsigned long cluster=0; cluster<k; cluster++) {
		for (unsigned long i=0; i<assignments[cluster].size(); i++) {
			double a = 0;
			for (unsigned long j=0; j<assignments[cluster].size(); j++) {
				if (i==j) continue;
				a += PLA::euclideanDistance(dataset[assignments[cluster][i]], dataset[assignments[cluster][j]]);
			}

			a /= (double) (assignments[cluster].size()-1);

			int startingCluster = 0;
			if (cluster==0) startingCluster = 1;
			double b = PLA::euclideanDistance(dataset[assignments[cluster][i]], centers[startingCluster]);
			for (unsigned long otherCluster=startingCluster; otherCluster<k; otherCluster++) {
				if (otherCluster==cluster) continue;
				double tmp = PLA::euclideanDistance(dataset[assignments[cluster][i]], centers[otherCluster]);
				if (tmp < b) {
					b = tmp;
				}
			}

			if (a < b) {
				silhouetteScore += 1 - (a/b);
			}
			else if (a > b) {
				silhouetteScore += (b/a) - 1;
			}
		}
	}
	return silhouetteScore / numOfVecs;
}

void Kmeans::assignStep(dmatrix& centers) {
	for (unsigned cluster=0; cluster<k; cluster++) {
		assignments[cluster].clear();
	}
	for (unsigned i=0; i<numOfVecs; i++) {
		double minDistance = PLA::euclideanDistanceSquared(centers[0], dataset[i]); 
		unsigned minIndex = 0;
		for (unsigned long j=1; j<k; j++) {
			double dist = PLA::euclideanDistanceSquared(centers[j], dataset[i]);
			if (dist < minDistance) {
				minDistance = dist;
				minIndex = j;
			}
		}
		assignments[minIndex].push_back(i);
	}
}
