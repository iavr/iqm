function [C, S] = nn_sub(Q, G, m, r)

% C: nearest neighbor indices, row-wise concatenated per subspace
% S: nearest neighbor (squared L2) distances, row-wise concatenated per subspace
% Q: query points
% G: grid (coarse codebook)
% m: # subspaces
% r: # nearest neighbors

if nargin < 4, r = 1; end

[d, D, N] = slices(Q, m);
R = m * r;

C = zeros(R, N, 'uint32');
S = zeros(R, N, 'single');

for i = 1:m
	s = slice(i, d, D);
	u = slice(i, r, R);
	[C(u,:), S(u,:)] = nn(Q(s,:), G(s,:), r);
end
