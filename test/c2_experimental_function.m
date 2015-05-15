addpath('../config');

times=zeros(10,3);
kkk = 1;

%for k=1000:1000:10000
k=14000;
    for iter=0:2
		  %--------------------------------
		  cfg.it = 20;         % # of iterations
		  cfg.K  = k;       % # of clusters
		  cfg.c  = 512;        % # of cells
		  cfg.m  = 16;         % # of subspaces      (unused currently)
		  cfg.k  = 256;        % fine codebook size  (unused currently)
		  cfg.w  = 512;         % search window
		  cfg.rr = 128;        % centroid rerank     (unused currently)
		  cfg.t  = 2;          % search target (# of points x N/K)
		  cfg.cn = [1 20];     % # of centroid neighbors
		  cfg.o  = 0;         % overlap threshold
		  %
		  %  cfg.dataset = 'siftsmall';
		  cfg.dataset = 'paris';
		  cfg.gen = false;
		  cfg.verbose = 1;

			cfg = c2_config(cfg);
			
		%	%--------------------------------
		%	fprintf('Learning codebooks\n');
		%	X = load_double_array('/home/user/sift/raw/paris50k.bin', 'single')';%xload(cfg.learn);
		%	u = cputime;
		%	[G,B] = c2_learn(cfg, X);
		%	fprintf('Learn time: %.3fs\n', cputime - u);
		%	xsave(cfg.grid, G);
		%	xsave(cfg.book, B);
   		%%
   		%%--------------------------------
		%	fprintf('Encoding vectors & codebooks\n');
		%	G = xload(cfg.grid);
		%	B = xload(cfg.book);
		%	X = load_double_array('/home/user/sift/raw/paris500k.bin', 'single')';
		%	u = cputime;
		%	[C,E] = c2_encode(cfg, G, B, X);
		%	fprintf('Encode time: %.3fs\n', cputime - u);
		%	xsave(cfg.cell, C);
		%	xsave(cfg.code, E);
		%	
		%	%--------------------------------
		%	fprintf('Inverting\n');
		%	C = xload(cfg.cell);
		%	%X = load_double_array(sprintf('/home/user/sift/raw/sift_%dk.bin', n), 'single');
		%	u = cputime;
		%	[P,Mi,M,I,cI] = c2_invert(cfg, C, X);
		%	fprintf('Invert time: %.3fs\n', cputime - u);
		%	xsave(cfg.pop, P);
		%	xsave(cfg.mean_i, Mi);
		%	xsave(cfg.mean, M);
		%	xsave(cfg.idx, I);
		%	save(cfg.inv, 'cI');
			
			%--------------------------------
%			fprintf('Initializing\n');
%			G = xload(cfg.grid);
%			B = xload(cfg.book);
%			E = xload(cfg.cell);
%			P = xload(cfg.pop);
%			u = cputime;
%			W = c2_init(cfg, G, B, E, P);
%			fprintf('Initialize time: %.3fs\n', cputime - u);
%			xsave(cfg.cen, W);
			
			%--------------------------------
			fprintf('Iterating\n');
			if cfg.synth
				X = xload(cfg.base);
				C = xload(cfg.cell);
			else
				X = [];
				C = [];
			end
			X = load_double_array('/home/user/sift/raw/paris500k.bin', 'single')';
			II = randperm(size(X,2), k);
			W = X(:,II);
			clear('II', 'X');
			X = [];
			%W  = single(csvread(sprintf('../exp/small/data/paris/initial_centroids_%d_%d.csv',k,iter))');%xload(cfg.cen);
			G  = xload(cfg.grid);
			B  = xload(cfg.book);
			E  = xload(cfg.cell);
			P  = xload(cfg.pop);
			Mi = xload(cfg.mean_i);
			M  = xload(cfg.mean);
			cfg.K = size(W, 2);
			u = cputime;
			[W,A] = c2_iter(cfg, W, G, B, E, P, Mi, M, X, C);
			times(kkk,(iter+1)) = cputime - u;	
			kkk = kkk + 1;
			fprintf('Iterate time: %.3fs\n', cputime - u);
			xsave(sprintf('./tests/final_%d_%d.f4', k, iter), W);
			xsave(cfg.asgn, A);
end
%end
csvwrite('./cmp_experiment/times.csv', times);
