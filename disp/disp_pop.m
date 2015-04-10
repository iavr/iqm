function disp_pop(it, f)

% it: iteration
% f: centroid population

K = length(f);
hist(f, K/10);
title(['iteration ' num2str(it-1) ', ' num2str(K) ' clusters']);
xlabel('cluster population (cells/cluster)')
ylabel('# of clusters')
