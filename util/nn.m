function [I, D] = nn(X, Q, k, type)

% just shortcut to yeal_nn, not requiring specific output arguments

if nargin < 3, k = 1; end
if nargin < 4, type = 2; end

[I, D] = yael_nn(X, Q, k, type);
