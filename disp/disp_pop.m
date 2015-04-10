function disp_pop(it, p)

% it: iteration
% p: centroid population

K = length(p);
hist(single(p), K/10);
title(['iteration ' num2str(it-1) ', ' num2str(K) ' clusters']);
xlabel('cluster population (cells/cluster)')
ylabel('# of clusters')
