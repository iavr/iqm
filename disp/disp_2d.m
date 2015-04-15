function disp_2d(it, X, A, p, W, var)

% it: iteration
% X: input data points
% A: centroid assignment per point (one-based)
% p: population per centroid
% W: centroids (mean vector per centroid)
% var: variance per centroid

K = size(W,2);        % # of centroids
a = find(A <= K);     % assigned points
u = find(A > K);      % unassigned points
f = find(p);          % non-empty centroids

% axes
clf
title(['iteration ' num2str(it-1) ', ' num2str(length(f)) ' non-empty clusters']);
mx = max(X(:));
mn = min(X(:));
axis([mn mx mn mx])
axis square
hold on

% points
gray = .5 * [1 1 1];
scatter(X(1,a), X(2,a), 100,'.k');          % assigned data points
scatter(X(1,u), X(2,u), 100, gray, '.');    % unassigned data points
scatter(W(1,f), W(2,f), 'or', 'filled');    % centers (points)

% circles
sigma = sqrt(var);
for i = 1:length(f)
	circle(W(:,f(i)), 2*sigma(f(i)));
end

hold off

% export pdf
eval(sprintf('print -dpdf ../anim/iter%02d', it))
