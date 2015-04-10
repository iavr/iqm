function [R, C] = enc_res_sub(G, X, m)

% R: residue
% C: encoding (coarse assignment)
% G: grid (coarse codebook)
% X: input data points
% m: # subspaces

[d, D, N] = slices(X, m);
type = int_type(size(G,2));
R = zeros(D, N, 'single');
C = zeros(m, N, type);
for i = 1:m
	s = slice(i, d, D);
	[R(s,:), C(i,:)] = enc_res(G(s,:), X(s,:));
end
