function [d, D, N] = slices(X, m)

% d: slice size (in rows)
% D: total length (in rows)
% N: # columns
% X: input data
% m: # slices (in rows)

[D, N] = size(X);
d = ceil(D / m);
