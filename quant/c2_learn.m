function [G, B] = c2_learn(cfg, X)

% G: grid (coarse codebook)
% B: fine codebook (for coarse)
% X: input data points

G = learn_sub(cfg, X, 2, cfg.c);
if cfg.sub,
	B = ex_learn(cfg, G);
else
	B = [];
end
