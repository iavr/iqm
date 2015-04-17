#include "br_kmeans.h"
#include <set>
#include <random>
#include <iostream>
#include <chrono>
#include <flann/flann.hpp>

using namespace std;

Kmeans::Kmeans(string dataFilename) {
	Reader::readFvecs(dataFilename, this->dataset);
	numOfVecs = dataset.size();
	dimension = dataset[0].size();
}

void Kmeans::runAlgorithm(unsigned long k, int l, int maxIter, bool verbose) {
	/** Initialization
	 */
	this->l = l;
	flann::Matrix<float> fDataset(new float[numOfVecs*dimension], numOfVecs, dimension);
	for (unsigned long i=0; i<numOfVecs; i++) {
		for (int j=0; j<dimension; j++) {
			fDataset[i][j] = dataset[i][j];
		}
	}
	cout << "Creating flann index" << endl;
	flann::Index<flann::L2<float> > index(fDataset, flann::KDTreeIndexParams(10));
	index.buildIndex();
	cout << "Flann index created" << endl;

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

	for (unsigned long i=0; i<numOfVecs; i++) {
		distances.push_back(-1);
		nearestCluster.push_back(k+1);
	}


	/** Main Loop
	 */
	chrono::high_resolution_clock::time_point t1, t2; 
	for (int iter=0; iter<maxIter; iter++) {
		/** Assign Step
		 */
		t1 = chrono::high_resolution_clock::now();
		assignStep(index, centers); 
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
			double sil = silhouette(centers);
			unsigned long total = 0;
			for (unsigned long c=0; c<k; c++) {
				total += assignments[c].size();
			}
			for (unsigned long n=0; n<numOfVecs; n++) {
				if (distances[n]==-1) {
					double minDist = PLA::euclideanDistance(dataset[n], centers[0]);
					int minIndex = 0;
					for (unsigned long c=0; c<k; c++) {
						double tmpDist = PLA::euclideanDistance(dataset[n], centers[c]);
						if (tmpDist<minDist) {
							minDist = tmpDist;
							minIndex = c;
						}
					}
					assignments[minIndex].push_back(n);
				}
			}
			unsigned long total2 = 0;
			for (unsigned long c=0; c<k; c++) {
				total2 += assignments[c].size();
			}

			cout << "Iteration #" << (iter+1) << ":"  << endl;
			cout << "\tKMeans objective score: " << score << endl;
			cout << "\tSilhouetter score: " << sil << endl;
			cout << "\tPercentage of assigned points " << (double) total/ numOfVecs << ", per: " << (double) total2/ numOfVecs << endl;
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

void Kmeans::assignStep(flann::Index<flann::L2<float> >& index, dmatrix& centers) {
	for (unsigned cluster=0; cluster<k; cluster++) {
		assignments[cluster].clear();
	}
	for (unsigned long i=0; i<numOfVecs; i++) {
		distances[i] = -1;
		nearestCluster[i] = k+1;
	}
	vector<vector<int> > indices;
	dmatrix dists;
	for (unsigned long cluster=0; cluster<k; cluster++) {
		flann::Matrix<float> query(new float[dimension], 1, dimension);
		for (int dim=0; dim<dimension; dim++) {
			query[0][dim] = centers[cluster][dim];
		}	
		int pointsFound = index.knnSearch(query, indices, dists, l, flann::SearchParams());

		for (unsigned long i=0; i<indices[0].size(); i++) {
			if (distances[indices[0][i]]==-1 || distances[indices[0][i]]>dists[0][i]) {
				distances[indices[0][i]] = dists[0][i];
				nearestCluster[indices[0][i]] = cluster;
			}
		}
	}
	for (unsigned long i=0; i<numOfVecs; i++) {
		if (distances[i]>-1) {
			assignments[nearestCluster[i]].push_back(i);
		}
	}

}
