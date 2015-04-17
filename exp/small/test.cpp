#include "kmeans.h"

using namespace std;

int main() {
	string filename = "/home/user/sift/raw/sift_learn.fvecs";
	Kmeans kmeans(filename);

	kmeans.runAlgorithm(100, 100, true);
}
