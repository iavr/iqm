function [C, S] = ex_nn_sub(B, E, X, m, r)

% C: nearest neighbor indices, row-wise concatenated per subspace
% S: nearest neighbor (squared L2) distances, row-wise concatenated per subspace
% B: fine codebook (for coarse)
% E: coarse codebook encoded by fine
% X: input data points
% m: # subspaces
% r: # nearest neighbors

if nargin < 4, r = 1; end

[d, D, N] = slices(X, m);
[z, Z, ~] = slices(E, m);
R = m * r;

C = zeros(R, N, 'uint32');
S = zeros(R, N, 'single');

for i = 1:m
	s = slice(i, d, D);
	t = slice(i, z, Z);
	u = slice(i, r, R);
	[C(u,:), S(u,:)] = ex_nn(B(s,:), E(t,:), X(s,:), r);
end
