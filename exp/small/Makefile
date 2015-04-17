SOURCES=kmeans.cpp pla.cpp reader.cpp test.cpp
BRSOURCES=br_kmeans.cpp pla.cpp reader.cpp br_test.cpp
FLAGS=-O3 -std=c++11

kmeans: $(SOURCES)
	g++ -o main $(SOURCES) $(FLAGS)

flann: $(BRSOURCES)
	g++ -o main $(BRSOURCES) $(FLAGS)
