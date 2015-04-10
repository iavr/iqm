function [C, E] = i2_encode(cfg, G, B, X)

% C: cell per point (coarse assignment)
% E: encoded points (fine assignment)
% G: grid (coarse codebook)
% B: fine codebook
% X: input data points

[R, C] = enc_res_sub(G, X, 2);
E = ex_encode(cfg, B, R);
