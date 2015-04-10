function B = learn_sub(cfg, X, m, k)

% B: codebook
% X: input data points
% m: # subspaces
% k: # centroids

[d, D, N] = slices(X, m);
B = zeros(D, k, 'single');
for i = 1:m
	s = slice(i, d, D);
	B(s,:) = cfg.cluster(X(s,:), k);
end
