function [C, S] = ex_nn_sub(Q, B, E, m, r)

% C: nearest neighbor indices, row-wise concatenated per subspace
% S: nearest neighbor (squared L2) distances, row-wise concatenated per subspace
% Q: query points
% B: fine codebook (for coarse)
% E: coarse codebook encoded by fine
% m: # subspaces
% r: # nearest neighbors

if nargin < 4, r = 1; end

[d, D, N] = slices(Q, m);
[z, Z, ~] = slices(E, m);
R = m * r;

C = zeros(R, N, 'uint32');
S = zeros(R, N, 'single');

for i = 1:m
	s = slice(i, d, D);
	t = slice(i, z, Z);
	u = slice(i, r, R);
	[C(u,:), S(u,:)] = ex_nn(Q(s,:), B(s,:), E(t,:), r);
end
