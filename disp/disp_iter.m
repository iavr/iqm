function disp_iter(it, f, P, A)

% it: iteration
% f: centroid population
% A: centroid assignment per point
% P: cell population

K = length(f);
u = find(P & A == K+1);
fprintf('Iteration %d\n', it)
fprintf('> %d/%d unassigned non-empty cells\n', length(u), prod(size(A)))
fprintf('> %d/%d unassigned points\n', sum(P(u)), sum(P(:)))
fprintf('> %d/%d empty centroids\n', length(find(f == 0)), K)
