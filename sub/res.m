function R = res(G, C, X)

% R: residue
% G: grid (coarse codebook)
% C: grid cells
% X: input data point(s)

% the number of elements in scalar/vector C must agree with the number of columns in column vector/matrix X, according to bsxfun rules.

R = bsxfun(@minus, X, G(:, C));
