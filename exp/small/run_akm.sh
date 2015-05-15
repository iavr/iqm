#!/bin/bash

dataset="/home/user/sift/raw/sift_base.bin"
k=$2
maxIter=10

ks=(1000 2000 3000 4000 5000 6000 7000 8000 9000 10000)
iters=(0 1 2 3 4)

for k in ${ks[@]}; do
	for iter in ${iters[@]}; do
		./akm "$dataset" "$k" "$maxIter" "./data/initial_centroids_$k""_$iter"".csv" >> time
		mv "./centers.csv" "./data/akm/final_centroids_$k""_$iter"".csv"
#		mv "./assignments.csv" "./data/assignments_$k""_$iter"".csv"
	done
done


comm="x=load_double_array('/home/user/sift/raw/sift_base.bin', 'single')'; ass=load_array('assignments.bin', 'uint32'); centers=load_double_array('computed_centers.bin', 'single')'; exp_computeDistortion(centers, x, ass)"

matlab -nodesktop -nosplash -r "$comm"
