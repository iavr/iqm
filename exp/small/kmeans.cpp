#include "kmeans.h"
#include <algorithm>
#include <limits>
#include <set>
#include <random>
#include <iostream>
#include <chrono>

using namespace std;

Kmeans::Kmeans(string dataFilename, unsigned long k, int maxIter, bool verbose) {
	Reader::readFvecs(dataFilename, this->dataset);
	numOfVecs = dataset.size();
	dimension = dataset[0].size();
	this->k = k;
	this->maxIter = maxIter;
	this->verbose = verbose;
	centers.reserve(k);
	labels.resize(numOfVecs,k);
	for (unsigned long i=0; i<k; i++) {
		assignments.push_back(uvector());
	}
}

void Kmeans::initializeExplicitly(dmatrix& initialCenters) {
	cout << "Initializing centers" << endl;
	centers.clear();
	for (unsigned long c=0; c<k; c++) {
		centers.push_back(dvector());	
		for (int dim=0; dim<dimension; dim++) {
			centers[c].push_back(initialCenters[c][dim]);
		}
	}
	cout << "Centers initialized. Computing original scores.." << endl;
	postInitialization();
}

bool comparator(pair<double, unsigned long> a, pair<double, unsigned long> b) {
	return a.first > b.first;
}

void Kmeans::initializePP() {
	std::uniform_int_distribution<int> distribution(0, numOfVecs-1);
	std::uniform_real_distribution<double> realDistribution(0.0, 1.0);
	default_random_engine generator;
	for (unsigned long i=0; i<k; i++) {
		centers.push_back(dvector());
	}	

	set<int> initialCentersIds;
	int id = distribution(generator);
	initialCentersIds.insert(id);
	for (int i=0; i<dimension; i++) {
		centers[0].push_back(dataset[id][i]);
	}

	for (unsigned long sample=1; sample<k; sample++) {
		cout << sample << endl;
		vector<pair<double, unsigned long> > probabilities;
		vector<double> distances;
		vector<unsigned long> indices;
		double minDist = numeric_limits<double>::infinity();
		unsigned long minIndex = numOfVecs;
		for (unsigned long i=0; i<numOfVecs; i++) {
			if (initialCentersIds.find(i)!=initialCentersIds.end()) continue;
			double dist = numeric_limits<double>::infinity();
			for (set<int>::iterator it=initialCentersIds.begin(); it!=initialCentersIds.end(); it++) {
				double tmp = PLA::euclideanDistance(dataset[i], dataset[(*it)]);
				if (tmp<dist) {
					dist = tmp;
				}
			}
			if (dist<minDist) {
				minDist = dist;
				minIndex = i;
			}
//			distances.push_back(dist*dist);
//			indices.push_back(i);
		}

//		double denominator = 0;
//		for (unsigned long i=0; i<distances.size(); i++) {
//			denominator += distances[i];
//		}
//		for (unsigned long i=0; i<distances.size(); i++) {
//			probabilities.push_back(make_pair(distances[i]/denominator, indices[i]));	
//		}
//
//		sort(probabilities.begin(), probabilities.end(), comparator);
//		for (unsigned long i=1; i<distances.size(); i++) {
//			probabilities[i].first += probabilities[i-1].first;
//		}		
//
//		double s = realDistribution(generator);	
//		for (unsigned long i=0; i<distances.size(); i++) {
//			if (s<=probabilities[i].first) {
				initialCentersIds.insert(minIndex);//probabilities[i].second);
				for (int dim=0; dim<dimension; dim++) {
					centers[sample].push_back(dataset[minIndex/*probabilities[i].second*/][dim]);
				}
//				break;
//			}
//		}
	}
}

void Kmeans::postInitialization() {
	if (verbose) {
		assignStep(centers); 
		double score = computeScore(centers);
		//double sil = silhouette();
		cout << "Initial centroids score:"  << endl;
		cout << "\tKMeans objective score: " << score << endl;
		//cout << "\tSilhouette score: " << sil << endl;
	}	
}
void Kmeans::initializeRandomly() {
	std::uniform_int_distribution<int> distribution(0, numOfVecs-1);
	default_random_engine generator;
	set<int> initialCentersIds;

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
		}
	}
	initialCentersIds.clear();
	postInitialization();
}

void Kmeans::runAlgorithm() {
	cout << "Starting main Loop" << endl;
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
			//double sil = silhouette();
			cout << "Iteration #" << (iter+1) << ":"  << endl;
			cout << "\tKMeans objective score: " << score << endl;
			//cout << "\tSilhouette score: " << sil << endl;
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
	for (unsigned cluster=0; cluster<k; cluster++) {
		assignments[cluster].clear();
	}

	for (unsigned i=0; i<numOfVecs; i++) {
		labels[i] = k;
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
