function [G, B] = i2_learn(cfg, X)

% G: grid (coarse codebook)
% B: fine codebook
% X: input data points

G = learn_sub(cfg, X, 2, cfg.c);
B = ex_learn(cfg, enc_res_sub(G, X, cfg.m));
