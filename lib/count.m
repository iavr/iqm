function C = count(X, K)
% Count elements assigned to each n-dimensional bin. Each row in X assigns one element to the bin of C with coordinates given by the row. For each row i, assignments should be integers with 1 <= X(i,j) <= K(j). This function fixes peculiarities of accumarray when X is a vector, and allows X to be a row vector, in which case C is a row vector as well.
%
% C: element counts per bin
% X: bin assignment per data point
% K: # of bins

row = false;
if ndims(X) == 2 && min(size(X)) < 2,  % vector
	if size(X,1) < 2, X = X'; row = true; end
	K = [K 1];
end
C = accumarray(X, 1, K);
if row, C = C'; end
