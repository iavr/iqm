%--------------------------------
cfg.dataset = 'sift';

cfg.in    = 0:0;        % input ids
cfg.it_m  = 20;         % # of iterations (maximum)
cfg.it_i  = 1;          % # of iterations (increment for saving; 0: no saving)
cfg.K_m   = 1000;       % # of clusters (maximum)
cfg.K     = 1000;       % # of clusters (current/increment)
cfg.c     = 256;        % # of cells

%--------------------------------
addpath('../config');
cfg = c2_config(cfg);

%--------------------------------
data = xload(cfg.base);
%data = load_double_array('/home/user/sift/raw/paris500k.bin', 'single');

n = 100000;
[batches,N] = slices(data',n);
inputs = max(cfg.in) + 1;
iters = floor(cfg.it_m / cfg.it_i) * inputs;
total = floor(cfg.K_m / cfg.K) * iters;
done = 0;
scores = [];

%--------------------------------
u = cputime;
for K = cfg.K:cfg.K:cfg.K_m
	for iter = cfg.it_i:cfg.it_i:cfg.it
		avg = 0;
		for in = cfg.in
			fprintf('Currently on k = %d, iteration %d #%d (%.2f%% completed)\n', K, iter, in, done / total * 100)
			done = done + 1;
			centers = xload(sprintf(cfg.iter, in, iter));
			for i = 1:batches
				[~,dist] = nn(data(:,slice(i,n,N)), centers);
				avg = avg + sum(dist);
			end
		end
		scores = [scores (avg / N / inputs)];
	end
end
fprintf('Evaluation time: %.3fs\n', cputime - u);
fprintf('Scores: [%s]\n', num2str(scores));
