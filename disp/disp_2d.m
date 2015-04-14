function disp_2d(it, X, A, W, var)

% it: iteration
% X: input data points
% A: centroid assignment per point
% W: centroids
% var: variance per centroid

K = size(W,2);        % # of centroids
a = find(A < K);      % assigned points
u = find(A >= K);     % unassigned points

% axes
clf
title(['iteration ' num2str(it-1) ', ' num2str(K) ' clusters']);
mx = max(X(:));
mn = min(X(:));
axis([mn mx mn mx])
axis square
hold on

% points
gray = .5 * [1 1 1];
scatter(X(1,a), X(2,a), 100,'.k');          % assigned data points
scatter(X(1,u), X(2,u), 100, gray, '.');    % unassigned data points
scatter(W(1,:), W(2,:), 'or', 'filled');    % centers (points)

% circles
sigma = sqrt(var);
for i = 1:length(sigma)
	circle(W(:,i), 2*sigma(i));
end

hold off

% export pdf
eval(sprintf('print -dpdf ../anim/iter%02d', it))
