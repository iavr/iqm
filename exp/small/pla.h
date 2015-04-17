#ifndef __PLA_H__
#define __PLA_H__

#include <vector>
#include <cmath>
#include <random>

typedef std::vector<float> dvector;
typedef std::vector<std::vector<float> > dmatrix;
typedef std::vector<unsigned> uvector;
typedef std::vector<std::vector<unsigned> > umatrix;

/** Class offering basic (primitime) linear algebra
 * operations
 */ 
class PLA {
public:
	static void defineNewGaussianGenerator(double mu, double std);
	static double dot(dvector a, dvector b);
	static void multiply(dvector& a, double val);
	static dmatrix multiply(dmatrix& a, dmatrix& b);
	static double norm(dvector a); 
	static dvector diff(dvector a, dvector b);
	static void add(dvector& a, dvector& b);
	static double euclideanDistance(dvector a, dvector b);
	static double euclideanDistance(double* a, double* b, int len);
	static double euclideanDistanceSquared(dvector a, dvector b);
	static dmatrix getUniformRandomMatrix(int N, int M);
	static dvector getUniformRandomVector(int N);
	static dmatrix getGaussianRandomMatrix(int N, int M);
	static dvector getGaussianRandomVector(int N);
	static dvector getUnitVector(int N);
	static void printMatrix(dmatrix& mat);
	static void printVector(dvector& vec);
	static void initializeGenerator();
	static double getUniformDouble();

	static std::mt19937 generator;
	static std::uniform_real_distribution<double> uniformDist;
	static std::normal_distribution<double> normalDist;
};
#endif
