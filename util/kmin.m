function [V, I] = kmin(X, r);

% just shortcut to yeal_kmin, not requiring specific input/output arguments; r fixed such that it is not greater than # columns of X (# points)

[V, I] = yael_kmin(X, min(r, size(X,1)));
