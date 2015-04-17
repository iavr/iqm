SOURCES=kmeans.cpp pla.cpp reader.cpp test.cpp
BRSOURCES=br_kmeans.cpp pla.cpp reader.cpp br_test.cpp
FLAGS=-O3 -std=c++11

kmeans: $(SOURCES)
	g++ -o kmeans $(SOURCES) $(FLAGS)

flann: $(BRSOURCES)
	g++ -o brkmeans $(BRSOURCES) $(FLAGS)

centroids: kmeans.cpp create_initial_centroids.cpp
	g++ -o create_centroids reader.cpp pla.cpp kmeans.cpp create_initial_centroids.cpp $(FLAGS)
