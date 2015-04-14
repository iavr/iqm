function disp_iter(it, p, P, A)

% it: iteration
% p: centroid population
% A: centroid assignment per point
% P: cell population

K = length(p);            % # of centroids
U = A >= K;               % unassigned cells
a = A(:); a(U(:)) = 0;    % cell assignments with 0 if unassigned
E = ~p(a + 1); E(U) = 1;  % cells assigned to empty (purged) centroids
E = reshape(E, size(P));
u = find(P & (U | E));    % non-empty cells ..
                          % .. unassigned or assigned to empty centroids

fprintf('Iteration %d\n', it)
fprintf('> %d/%d unassigned non-empty cells\n', length(u), prod(size(A)))
fprintf('> %d/%d unassigned points\n', sum(P(u)), sum(P(:)))
fprintf('> %d/%d empty centroids\n', length(find(p == 0)), K)
