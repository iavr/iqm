function [V, I] = kmin(X, k);

% just shortcut to yeal_kmin, not requiring specific input/output arguments; k fixed such that it is not greater than # columns of X (# points)

[V, I] = yael_kmin(X, min(k, size(X,1)));
