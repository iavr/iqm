%--------------------------------
cfg.dataset = 'siftsmall';
cfg.dataset = 'sift';
cfg.gen = false;
cfg.verbose = 0;
%
cfg.in    = 0:0;        % input ids
cfg.it_m  = 20;         % # of iterations (maximum)
cfg.it_i  = 1;          % # of iterations (increment for saving; 0: no saving)
cfg.K_m   = 1000;       % # of clusters (maximum)
cfg.K     = 1000;       % # of clusters (current/increment)
cfg.c     = 256;        % # of cells
cfg.sub   = false;      % quantize sub-centroids
cfg.m     = 16;         % # of subspaces      (only if sub)
cfg.k     = 256;        % fine codebook size  (only if sub)
cfg.w     = 16;         % search window
cfg.t     = 5;          % search target (# of points x N/K)
%  cfg.cn    = [6 20];     % # of centroid neighbors
%  cfg.o     = .6;         % overlap threshold
cfg.cn    = [1 0];      % # of centroid neighbors
cfg.o     = .0;         % overlap threshold

%  %--------------------------------
%  cfg.dataset = '2d_uni';
%  cfg.dataset = '2d_gm';
%  cfg.gen = false;
%  cfg.verbose = 2;
%  %
%  cfg.in    = 0:0;        % input ids
%  cfg.it_m  = 10;         % # of iterations (maximum)
%  cfg.it_i  = 0;          % # of iterations (increment for saving; 0: no saving)
%  cfg.K_m   = 24;         % # of clusters (maximum)
%  cfg.K     = 24;         % # of clusters (current/increment)
%  cfg.c     = 64;         % # of cells
%  cfg.sub   = false;      % quantize sub-centroids
%  cfg.m     = 2;          % # of subspaces      (only if sub)
%  cfg.k     = 16;         % fine codebook size  (only if sub)
%  cfg.w     = 24;         % search window
%  cfg.t     = 2;          % search target (# of points x N/K)
%  cfg.cn    = [4 10];     % # of centroid neighbors
%  cfg.o     = .6;         % overlap threshold

%--------------------------------
addpath('../config');
cfg = c2_config(cfg);

%  %--------------------------------
%  fprintf('Learning codebooks\n');
%  X = xload(cfg.learn);
%  u = cputime;
%  [G,B] = c2_learn(cfg, X);
%  fprintf('Learn time: %.3fs\n', cputime - u);
%  xsave(cfg.grid, G);
%  xsave(cfg.book, B);
%  %
%  %--------------------------------
%  fprintf('Encoding vectors & codebooks\n');
%  G = xload(cfg.grid);
%  B = xload(cfg.book);
%  X = xload(cfg.base);
%  u = cputime;
%  [C,E] = c2_encode(cfg, G, B, X);
%  fprintf('Encode time: %.3fs\n', cputime - u);
%  xsave(cfg.cell, C);
%  xsave(cfg.code, E);
%  %
%  %--------------------------------
%  fprintf('Inverting\n');
%  C = xload(cfg.cell);
%  X = xload(cfg.base);
%  u = cputime;
%  [P,Mi,M,I,cI] = c2_invert(cfg, C, X);
%  fprintf('Invert time: %.3fs\n', cputime - u);
%  xsave(cfg.pop, P);
%  xsave(cfg.mean_i, Mi);
%  xsave(cfg.mean, M);
%  xsave(cfg.idx, I);
%  save(cfg.inv, 'cI');

for K = cfg.K:cfg.K:cfg.K_m
	fprintf('Clustering with k = %d\n', K);
	cfg.K = K;
	for in = 0:cfg.in
		fprintf('Using initial input %d\n', in);
		cfg.in = in;
		cfg = c2_config(cfg);

		%--------------------------------
		if cfg.verbose, fprintf('Initializing\n'); end
		G = xload(cfg.grid);
		B = xload(cfg.book);
		P = xload(cfg.pop);
		u = cputime;
		W = c2_init(cfg, G, B, P);
		if cfg.verbose, fprintf('Initialize time: %.3fs\n', cputime - u); end
		xsave(cfg.cen, W);

		%--------------------------------
		if cfg.verbose, fprintf('Iterating\n'); end
		if cfg.synth
			X = xload(cfg.base);
			C = xload(cfg.cell);
		else
			X = [];
			C = [];
		end
		W  = xload(cfg.cen);
		G  = xload(cfg.grid);
		B  = xload(cfg.book);
		E  = xload(cfg.code);
		P  = xload(cfg.pop);
		Mi = xload(cfg.mean_i);
		M  = xload(cfg.mean);
		cfg.K = size(W, 2);
		u = cputime;
		[W,A,times] = c2_iter(cfg, W, G, B, E, P, Mi, M, X, C);
		fprintf('Iterate time: %.3fs\n', cputime - u);
		xsave(sprintf(cfg.cen, in), W);
		xsave(sprintf(cfg.asgn, in), A);

	end
end
