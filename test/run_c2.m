function run_c2()
    data = fvecs_read('/home/user/sift/raw/sift_learn.fvecs');
    data = single(data);
    results = [];
    times = [];
    for k=1000:1000:10000
        for i=0:4
               centers=csvread(sprintf('./../exp/small/data/initial_centroids_%d_%d.csv', k, i));
               u = cputime;
               c2_test_function(centers, k, sprintf('./../exp/small/data/final_centroids_%d_%d.f4', k, i), sprintf('./../exp/small/data/assignments_%d_%d.u4', k, i));
               times = [times (cputime - u)];
%              csvwrite(sprintf('./final_centroids_%d_%d.csv', k, i), C);
               %csvwrite(sprintf('./assignments_%d_%d.csv', k, i), I);
        end
    end
    results = times;
    csvwrite('./total_results_cpuinfo.csv', results);
