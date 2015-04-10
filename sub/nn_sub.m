function [C, S] = nn_sub(G, X, m, k)

% C: nearest neighbor indices, row-wise concatenated per subspace
% S: nearest neighbor (squared L2) distances, row-wise concatenated per subspace
% G: grid (coarse codebook)
% X: input data points
% m: # subspaces
% k: # nearest neighbors

if nargin < 4, k = 1; end

[d, D, N] = slices(X, m);
C = zeros(m * k, N, 'uint32');
S = zeros(m * k, N, 'single');
for i = 1:m
	s = slice(i, d, D);
	t = slice(i, k, m * k);
	[C(t,:), S(t,:)] = nn(G(s,:), X(s,:), k);
end
