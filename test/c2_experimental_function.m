addpath('../config');

loop_times_1 = [];
loop_times_2 = [];
for k=1000:1000:10000
	avg_time_1 = 0;
	avg_time_2 = 0;
	for iter=0:0	
		%  %--------------------------------
		%  cfg.it = 20;         % # of iterations
		%  cfg.K  = 1000;       % # of clusters
		%  cfg.c  = 512;        % # of cells
		%  cfg.m  = 16;         % # of subspaces      (unused currently)
		%  cfg.k  = 256;        % fine codebook size  (unused currently)
		%  cfg.w  = 64;         % search window
		%  cfg.rr = 128;        % centroid rerank     (unused currently)
		%  cfg.t  = 5;          % search target (# of points x N/K)
		%  cfg.cn = [6 20];     % # of centroid neighbors
		%  cfg.o  = .6;         % overlap threshold
		%  %
		%  %  cfg.dataset = 'siftsmall';
		%  cfg.dataset = 'sift';
		%  cfg.gen = false;
		%  cfg.verbose = 1;
		
		%--------------------------------
		cfg.it = 20;        % # of iterations
		cfg.K  = k;        % # of clusters
		cfg.c  = 256;        % # of cells
		cfg.m  = 16;         % # of subspaces      (unused currently)
		cfg.k  = 256;        % fine codebook size  (unused currently)
		cfg.w  = 5;        % search window
		cfg.rr = 128;        % centroid rerank     (unused currently)
		cfg.t  = 3;         % search target (# of points x N/K)
		cfg.cn = [1 10];    % # of centroid neighbors
		cfg.o  = 0;        % overlap threshold
		%
		%cfg.dataset = '2d_gm';
		cfg.dataset = 'sift';
		cfg.gen = false;
		cfg.verbose = 0;
		
		%--------------------------------
		cfg = c2_config(cfg);
		
		%  %--------------------------------
		%fprintf('Learning codebooks\n');
		%X = xload(cfg.learn);
		%u = cputime;
		%[G,B] = c2_learn(cfg, X);
		%fprintf('Learn time: %.3fs\n', cputime - u);
		%xsave(cfg.grid, G);
		%xsave(cfg.book, B);
		%
		%--------------------------------
		%fprintf('Encoding vectors & codebooks\n');
		%G = xload(cfg.grid);
		%B = xload(cfg.book);
		%X = xload(cfg.base);
		%u = cputime;
		%[C,E] = c2_encode(cfg, G, B, X);
		%fprintf('Encode time: %.3fs\n', cputime - u);
		%xsave(cfg.cell, C);
		%xsave(cfg.code, E);
		  %
		%  %--------------------------------
		%fprintf('Inverting\n');
		%C = xload(cfg.cell);
		%X = xload(cfg.base);
		%u = cputime;
		%[P,Mi,M,I,cI] = c2_invert(cfg, C, X);
		%fprintf('Invert time: %.3fs\n', cputime - u);
		%xsave(cfg.pop, P);
		%xsave(cfg.mean_i, Mi);
		%xsave(cfg.mean, M);
		%xsave(cfg.idx, I);
		%save(cfg.inv, 'cI');
		
		%--------------------------------
		%fprintf('Initializing\n');
		%G = xload(cfg.grid);
		%B = xload(cfg.book);
		%E = xload(cfg.cell);
		%P = xload(cfg.pop);
		%u = cputime;
		%W = c2_init(cfg, G, B, E, P);
		%fprintf('Initialize time: %.3fs\n', cputime - u);
		%xsave(cfg.cen, W);
		
		%--------------------------------
		fprintf('Iterating\n');
		if cfg.synth
			X = xload(cfg.base);
			C = xload(cfg.cell);
		else
			X = [];
			C = [];
		end
		%W  = xload(cfg.cen);
		X = xload(cfg.learn);
		W  = single(csvread(sprintf('/home/user/ikm/exp/small/data/initial_centroids_%d_%d.csv',k,iter))');%xload(cfg.cen);

		G  = xload(cfg.grid);
		B  = xload(cfg.book);
		E  = xload(cfg.cell);
		P  = xload(cfg.pop);
		Mi = xload(cfg.mean_i);
		M  = xload(cfg.mean);
		cfg.K = size(W, 2);
		u = cputime;
		[W,A,times] = c2_iter(cfg, W, G, B, E, P, Mi, M, X, C, iter);
		u = cputime - u;
		avg_time_1 = avg_time_1 + sum(times);
		avg_time_2 = avg_time_2 + u;
		fprintf('Iterate time: %.3fs (actual: %.3fs)\n', u, sum(times));
		xsave(sprintf('./results/final_%d_%d.f4', k, iter), W);
		xsave(cfg.asgn, A);
	end
	loop_times_1 = [loop_times_1 (avg_time_1/3)];
	loop_times_2 = [loop_times_2 (avg_time_2/3)];
end
