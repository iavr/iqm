#include "akm.h"
#include <set>
#include <random>
#include <iostream>
#include <chrono>
#include <flann/flann.hpp>
#include <flann/flann.h>

using namespace std;

Kmeans::Kmeans(string dataFilename, unsigned long k, int maxIter, bool verbose) {
	Reader::readVector(dataFilename, this->dataset);
	numOfVecs = dataset.size();
	dimension = dataset[0].size();
	this->k = k;
	this->maxIter = maxIter;
	this->verbose = verbose;
	centers.reserve(k);
	labels.resize(numOfVecs,k);
	distances.resize(numOfVecs, -1);
	for (unsigned long i=0; i<k; i++) {
		assignments.push_back(uvector());
	}
}
void Kmeans::saveCenters(int iter) {
	ostringstream oss;
	oss << "centers_iter_" << (iter+1/) << ".csv";
	ofstream outCenters(oss.str());
	for (unsigned i=0; i<k; i++) {
		outCenters << centers[i][0];
		for (int j=1; j<dimension; j++) {
			outCenters << ", " << centers[i][j];
		}
		outCenters << "\n";
	}
	outCenters.close();
}

void Kmeans::initializeRandomly() {
	std::uniform_int_distribution<int> distribution(0, numOfVecs-1);
	default_random_engine generator;
	set<int> initialCentersIds;

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
		}
	}
	initialCentersIds.clear();
}

void Kmeans::initializeExplicitly(dmatrix& initialCenters) {
	//cout << "Initializing centers" << endl;
	centers.clear();
	for (unsigned long c=0; c<k; c++) {
		centers.push_back(dvector());	
		for (int dim=0; dim<dimension; dim++) {
			centers[c].push_back(initialCenters[c][dim]);
		}
	}
	//cout << "Centers initialized. Computing original scores.." << endl;
}

void Kmeans::postInitialization() {
}

void Kmeans::runAlgorithm() {
	/** Initialization
	 */
	chrono::high_resolution_clock::time_point t1, t2; 
	if (verbose) {
//		assignStep(index, centers);
//		unsigned long total = 0;
//		for (unsigned long c=0; c<k; c++) {
//			total += assignments[c].size();
//		}
//		for (unsigned long n=0; n<numOfVecs; n++) {
////			if (distances[n]==-1) {
//				double minDist = PLA::euclideanDistance(dataset[n], centers[0]);
//				int minIndex = 0;
//				for (unsigned long c=0; c<k; c++) {
//					double tmpDist = PLA::euclideanDistance(dataset[n], centers[c]);
//					if (tmpDist<minDist) {
//						minDist = tmpDist;
//						minIndex = c;
//					}
//				}
//				assignments[minIndex].push_back(n);
////			}
//		}
//		unsigned long total2 = 0;
//		for (unsigned long c=0; c<k; c++) {
//			total2 += assignments[c].size();
//		}
//
//		assignStep();
//		double score = computeScore(centers);
//		//double sil = silhouette(centers);
//		cout << "Initial cluster score:"  << endl;
//		cout << "\tKMeans objective score: " << score << endl;
//		//cout << "\tSilhouetter score: " << sil << endl;
//		//cout << "\tPercentage of assigned points " << (double) total/ numOfVecs << ", per: " << (double) total2/ numOfVecs << endl;
	}	

	/** Main Loop
	 */
	t1 = chrono::high_resolution_clock::now();
	for (int iter=0; iter<maxIter; iter++) {
		//cout << "Iter #" << (iter+1) << endl;
		/** Assign Step
		 */
		assignStep(centers); 

		/** Update Step
		 */
		centers.clear();
		for (unsigned cluster=0; cluster<k; cluster++) {
			dvector newCenter(dimension, 0);
			for (unsigned i=0; i<assignments[cluster].size(); i++) {
				PLA::add(newCenter, dataset[assignments[cluster][i]]);
			}
			PLA::multiply(newCenter, 1.0/ assignments[cluster].size());
			centers.push_back(newCenter);
		}

		/** Compute Score
		 */
		if (verbose) {
			unsigned long total = 0;
			for (unsigned long c=0; c<k; c++) {
				total += assignments[c].size();
			}
		//	for (unsigned long n=0; n<numOfVecs; n++) {
//		//		if (distances[n]==-1) {
		//			double minDist = PLA::euclideanDistance(dataset[n], centers[0]);
		//			int minIndex = 0;
		//			for (unsigned long c=0; c<k; c++) {
		//				double tmpDist = PLA::euclideanDistance(dataset[n], centers[c]);
		//				if (tmpDist<minDist) {
		//					minDist = tmpDist;
		//					minIndex = c;
		//				}
		//			}
		//			assignments[minIndex].push_back(n);
//		//		}
		//	}
		//	unsigned long total2 = 0;
		//	for (unsigned long c=0; c<k; c++) {
		//		total2 += assignments[c].size();
		//	}
//			assignStep();
			saveCenters(iter);

//			double score = computeScore(centers);
//			//double sil = silhouette(centers);
//			cout << "Iteration #" << (iter+1) << ":"  << endl;
//			cout << "\tKMeans objective score: " << score << endl;
//			//cout << "\tSilhouetter score: " << sil << endl;
//			cout << "\tPercentage of assigned points " << (double) total/ numOfVecs << endl;//", per: " << (double) total2/ numOfVecs << endl;
			//cout <<	"\tTotal time: " << (assignTime.count() + updateTime.count()) << "s, update time: " << updateTime.count() << "s, assign time: " << assignTime.count() << "s" << endl;
		}
	}
	t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> loopTime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
	//cout << loopTime.count() << endl;
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

double Kmeans::silhouette() {
	double silScore = 0;
	for (unsigned long i=0; i<numOfVecs; i++) {
		double a = 0;
		unsigned myCluster = labels[i];
		for (unsigned long j=0; j<assignments[myCluster].size(); j++) {
			if (assignments[myCluster][j]==i) continue;
			a += PLA::euclideanDistance(dataset[i], dataset[assignments[myCluster][j]]);
		}
		a /= (double) (assignments[myCluster].size() - 1);

		double b = std::numeric_limits<double>::infinity();
		for (unsigned long otherCluster=0; otherCluster<k; otherCluster++) {
			if (otherCluster==myCluster) continue;
			double tmp = 0;
			for (unsigned long j=0; j<assignments[otherCluster].size(); j++) {
				tmp += PLA::euclideanDistance(dataset[i], dataset[assignments[otherCluster][j]]);
			}
			tmp /= (double) assignments[otherCluster].size();
			if (tmp < b)
				b = tmp;
		}

		if (a < b) {
			silScore += 1 - (a/b);
		}
		else if (b < a) {
			silScore += (b/a) - 1;
		}
	}
	return silScore / numOfVecs;
}

void Kmeans::assignStep(dmatrix& centers) {
	flann::Matrix<float> fDataset(new float[k*dimension], k, dimension);
	for (unsigned long i=0; i<k; i++) {
		for (int j=0; j<dimension; j++) {
			fDataset[i][j] = centers[i][j];
		}
	}
	flann::KDTreeIndexParams params(10);
	flann::Index<flann::L2<float> > index(fDataset, params);
	index.buildIndex();
	for (unsigned cluster=0; cluster<k; cluster++) {
		assignments[cluster].clear();
	}

	for (unsigned long i=0; i<numOfVecs; i++) {
		distances[i] = -1;
		labels[i] = k+1;
	}

	vector<vector<int> > indices;
	dmatrix dists;
	for (unsigned long i=0; i<numOfVecs; i++) {
		flann::Matrix<float> query(new float[dimension], 1, dimension);
		for (int dim=0; dim<dimension; dim++) {
			query[0][dim] = dataset[i][dim];
		}	
		int pointsFound = index.knnSearch(query, indices, dists, 1, flann::SearchParams());
		if (pointsFound==0) continue;

		distances[i] = dists[0][0];
		labels[i] = indices[0][0];
		assignments[indices[0][0]].push_back(i);
		delete query.ptr();
		
	}
	delete fDataset.ptr();
}

void Kmeans::assignStep() {
	for (unsigned cluster=0; cluster<k; cluster++) {
		assignments[cluster].clear();
	}

	for (unsigned i=0; i<numOfVecs; i++) {
		labels[i] = 0;
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
		labels[i] = minIndex;
	}
}
