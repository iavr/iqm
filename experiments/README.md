Experimental code for the k-means clustering algorithm and a variation of Broder's "Scalable K-Means by Ranked Retrieval", where we utilize FLANN
in order to find the closest l points to each centroid.

test.cpp contains an example usage of the plain kmeans algorithm.
You can compile and run this example by issuing
> make kmeans

br_test.cpp contains an example usage of the FLANN algorithm.
You can compile and run this example by issuing
> make flann
Note that this expects FLANN to be installed in your system and be available in standard include directories.

At the moment only input for the fvecs format is supported.
