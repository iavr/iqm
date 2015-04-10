function E = enc_sub(B, X, m)

% E: encoded points (assignment)
% B: codebook
% X: input data points
% m: # subspaces

[d, D, N] = slices(X, m);
type = int_type(size(B,2));
E = zeros(m, N, type);
for i = 1:m
	s = slice(i, d, D);
	E(i,:) = enc(B(s,:), X(s,:));
end
