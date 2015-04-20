SOURCES=kmeans.cpp pla.cpp reader.cpp test.cpp
BRSOURCES=br_kmeans.cpp vector_io.hpp pla.cpp reader.cpp br_main.cpp
AKMSOURCES=akm.cpp vector_io.hpp pla.cpp reader.cpp akm_main.cpp
FLAGS=-O3 -std=c++11

kmeans: $(SOURCES)
	g++ -o kmeans $(SOURCES) $(FLAGS)

broder: $(BRSOURCES)
	g++ -o brkmeans $(BRSOURCES) $(FLAGS)

akm: $(AKMSOURCES)
	g++ -o akm $(AKMSOURCES) $(FLAGS)

centroids: kmeans.cpp create_initial_centroids.cpp
	g++ -o create_centroids reader.cpp pla.cpp kmeans.cpp create_initial_centroids.cpp $(FLAGS)
