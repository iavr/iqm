#!/bin/bash

dataset="/home/user/sift/raw/sift_base.bin"
k=$2
tau=-1 # this will set the value of the points to search for to n/k
maxIter=20

ks=(10000) # 2000 3000 4000 5000 6000 7000 8000 9000 10000)
iters=(0 1) # 1 2 3 4)

for k in ${ks[@]}; do
	for iter in ${iters[@]}; do
		./brkmeans "$dataset" "$k" "$tau" "$maxIter" "./data/initial_centroids_$k""_$iter"".csv" >> time
		mv "./centers.csv" "./data/paris/final_centroids_$k""_$iter"".csv"
#		mv "./assignments.csv" "./data/assignments_$k""_$iter"".csv"
	done
done

#make broder

