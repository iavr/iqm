function r = recall(cfg, G, R)

% r: recall@r measurement
% G: ground truth indices per query (in columns)
% R: returned indices per query (in columns)

s = cumsum(bsxfun(@eq, R, G(1,:)), 1);
r = mean(s(cfg.R, :), 2)';
