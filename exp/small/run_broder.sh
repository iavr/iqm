#!/bin/bash

dataset=$1
k=$2
tau=$3 # this will set the value of the points to search for to n/k
maxIter=$4

#make broder

./brkmeans "$dataset" "$k" "$tau" "$maxIter"

comm="x=load_double_array('/home/user/sift/raw/sift_base.bin', 'single')'; ass=load_array('assignments.bin', 'uint32'); centers=load_double_array('computed_centers.bin', 'single')'; exp_computeDistortion(centers, x, ass)"

matlab -nodesktop -nosplash -r "$comm"
