#include "pla.h"
#include <iostream>

using namespace std;

mt19937 PLA::generator(1990);
uniform_real_distribution<double> PLA::uniformDist(-20.0, 20.0);
normal_distribution<double> PLA::normalDist(0.0, 0.1);

void PLA::defineNewGaussianGenerator(double mu, double std) {
	normal_distribution<double> newNormalDist(mu, std);
	PLA::normalDist = newNormalDist;
}

double PLA::dot(dvector a, dvector b) {
	double dotProduct = 0;
	for (unsigned i=0; i<a.size(); i++) {
		dotProduct += a[i] * b[i];
	}
	return dotProduct;
}

void PLA::multiply(dvector& a, double val) {
	for (unsigned i=0; i<a.size(); i++) {
		a[i] *= val;
	}
}

double PLA::euclideanDistance(dvector a, dvector b) {
	return norm(diff(a, b));	
}

double PLA::euclideanDistance(double* a, double* b, int len) {
	double sum = 0;

	for (int i=0; i<len; i++) {
		double tmp = a[i] - b[i];
		sum += tmp * tmp;
	}
	return sqrt(sum);
}

double PLA::euclideanDistanceSquared(dvector a, dvector b) {
	dvector c = diff(a, b);
	double sum = 0;
	for (unsigned i=0; i<c.size(); i++) {
		sum += c[i]*c[i];
	}
	return sum;
}

double PLA::norm(dvector a) {
	return sqrt(dot(a, a));
}

dvector PLA::diff(dvector a, dvector b) {
	dvector difference;
	for (unsigned i=0; i<a.size(); i++) {
		difference.push_back(a[i] - b[i]);
	}
	return difference;
}

void PLA::add(dvector& a, dvector& b) {
	for (unsigned i=0; i<a.size(); i++) {
		a[i] += b[i];
	}
}
dmatrix PLA::getUniformRandomMatrix(int N, int M) {
	dmatrix rMatrix;
	rMatrix.reserve(N);
	for (int i=0; i<N; i++) {
		rMatrix.push_back(getUniformRandomVector(M));
	}
	return rMatrix;
}

dvector PLA::getUniformRandomVector(int N) {
	dvector rVector;
	rVector.reserve(N);
	for (int i=0; i<N; i++) {
		rVector.push_back(uniformDist(generator));
	}
	return rVector;
}
dmatrix PLA::getGaussianRandomMatrix(int N, int M) {
	dmatrix rMatrix;
	rMatrix.reserve(N);
	for (int i=0; i<N; i++) {
		rMatrix.push_back(getGaussianRandomVector(M));
	}
	return rMatrix;
}

dvector PLA::getGaussianRandomVector(int N) {
	dvector rVector;
	rVector.reserve(N);
	for (int i=0; i<N; i++) {
		rVector.push_back(normalDist(generator));
	}
	return rVector;
}
dvector PLA::getUnitVector(int N) {
	dvector rVector = getUniformRandomVector(N);
	double norm = PLA::norm(rVector);
	for (int i=0; i<N; i++) {
		rVector[i] /= norm;
	}
	return rVector;
}

dmatrix PLA::multiply(dmatrix& a, dmatrix& b) {
	dmatrix result;
	result.resize(a.size());
	for (unsigned i=0; i<a.size(); i++) {
		for (unsigned j=0; j<b[0].size(); j++) {
			double dot = 0;
			for (unsigned c=0; c<b.size(); c++) {
				dot += a[i][c] * b[c][j];
			}
			result[i].push_back(dot);
		}
	}
	return result;
}

void PLA::printVector(dvector& vec) {
	cout << "(" << vec[0];
	for (unsigned i=1; i<vec.size(); i++) {
		cout << ", " << vec[i];
	}
	cout << ")";
}
void PLA::printMatrix(dmatrix& mat) {
	for (unsigned i=0; i<mat.size(); i++) {
		printVector(mat[i]);
		cout << endl;
	}
}
void PLA::initializeGenerator() {
	generator.seed(random_device{}());
}
double PLA::getUniformDouble() {
	return (20+PLA::uniformDist(PLA::generator))/40;
}
