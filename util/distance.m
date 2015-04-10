function d = dist(X, Q)

% d: all pairwise distances
% X: data points (columns)
% Q: query points (columns)

Xn = sum(X.^2) / 2;
Qn = sum(Q.^2) / 2;
d = bsxfun(@plus, Qn, bsxfun(@minus, Xn', X'*Q));
