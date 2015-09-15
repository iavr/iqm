% k nearest neighbors of a set of query points according to L2 distance
%
% Usage: [I,D] = nn(Q, X, k)
%   Q                query points (one vector per column)
%   X                data points to be searched (one vector per column)
%   k  (default:1)   number of nearest neigbors needed
%
% Returns
%   I  the vector index of the nearest neighbors
%   D  the corresponding *square* distances
function [I, D] = nn_l2(Q, X, k, slicesize)

if ~exist('k'), k = 1; end
if ~exist('slicesize'), slicesize = 100000; end

assert(size(Q,1) == size(X,1));
q = size(Q,2);
n = size(X,2);

Qn = sum(Q.^2) / 2;
Xn = sum(X.^2) / 2;
%  S = bsxfun(@plus, Qn', bsxfun(@minus, Xn, Q'*X));
%  S = repmat(Qn',[1 n]) + (repmat(Xn,[q 1]) - Q'*X);
A = repmat(Qn',[1 n]);
B = repmat(Xn, [q 1]);
C = Q'*X;
S = A + B - C;

if k == 1
	[D,I] = min(S, [], 2);
else
	[D,I] = sort(S, 2);
	D = D(:, 1:k);
	I = I(:, 1:k);
end

D = D' * 2;
I = I';

