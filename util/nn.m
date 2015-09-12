function [I, D] = nn(Q, X, r, type)

% I: nearest neighbor indices
% D: nearest neighbor (squared) distances
% Q: query points
% X: data points
% r: # nearest neighbors
% type: distance type (see yael_nn, default L2)

% shortcut to yeal_nn, not requiring specific output arguments
% caution: changes order of Q, X!!!

if nargin < 3, r = 1; end
if nargin < 4, type = 2; end

[I, D] = yael_nn(X, Q, min(r, size(X,2)), type);
