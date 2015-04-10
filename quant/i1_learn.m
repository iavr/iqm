function [G, B] = i1_learn(cfg, X)

% G: grid (coarse codebook)
% B: fine codebook
% X: input data points

G = cfg.cluster(X, cfg.c);
B = ex_learn(cfg, enc_res(G, X));
