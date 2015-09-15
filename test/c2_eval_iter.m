%--------------------------------
cfg.dataset = 'sift';

cfg.unit  = 'cpu';      % processing unit (cpu or gpu)
cfg.in    = 0:4;        % input ids
cfg.it_m  = 20;         % # of iterations (maximum)
cfg.it_i  = 1;          % # of iterations (increment for saving; 0: no saving)
cfg.K_m   = 2000;       % # of clusters (maximum)
cfg.K_i   = 1000;       % # of clusters (increment)
cfg.c     = 256;        % # of cells

%--------------------------------
addpath('../config');
cfg = c2_config(cfg, true);

%--------------------------------
X = xload(cfg.base);
%  X = load_double_array('/home/user/sift/raw/paris500k.bin', 'single');

a = cfg.alloc;
X = a.on(X);

n = 100000;  % max # of vectors to process so they fit in memory
[batches,N] = slices(X',n);
inputs = max(cfg.in) + 1;
iters = floor(cfg.it_m / cfg.it_i);
kappa = floor(cfg.K_m / cfg.K_i);
total = inputs * iters * kappa;
scores = zeros(iters, kappa, 'single');
done = 0;

%--------------------------------
u = cputime;
for k = 1:kappa
	K = k * cfg.K_i;
	for i = 1:iters
		iter = i * cfg.it_i;
		avg = 0;
		for in = cfg.in
			msg = 'Currently on k = %d, iteration %d #%d (%.2f%% completed)\n';
			fprintf(msg, K, iter, in, done / total * 100)
			done = done + 1;
			centers = a.on(xload(sprintf(cfg.iter, K, in, iter)));
			for j = 1:batches
				[~,dist] = nn_l2(X(:,slice(j,n,N)), centers);
				avg = avg + sum(dist);
			end
		end
		scores(i, k) = avg / N / inputs;
	end
end
fprintf('\nEvaluation time: %.3fs\n', cputime - u);
scores
