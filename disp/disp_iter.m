function disp_iter(it, p, P, A)

% it: iteration
% p: centroid population
% A: centroid assignment per point
% P: cell population

K = length(p);
u = find(P & A == K);
fprintf('Iteration %d\n', it)
fprintf('> %d/%d unassigned non-empty cells\n', length(u), prod(size(A)))
fprintf('> %d/%d unassigned points\n', sum(P(u)), sum(P(:)))
fprintf('> %d/%d empty centroids\n', length(find(p == 0)), K)
