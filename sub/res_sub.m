function R = res_sub(G, C, X, m)

% R: residue
% G: grid (coarse codebook)
% C: grid cells
% X: input data point(s)
% m: # subspaces

[d, D, N] = slices(X, m);
K = size(C,1);
if N == 1, J = K;
elseif K == 1 | K == N, J = N;
else error('Inconsistent dimensions.')
end
for i = 1:m
	s = slice(i, d, D);
	R(s,:) = res(G(s,:), C(:,i), X(s,:));
end
