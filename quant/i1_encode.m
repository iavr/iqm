function [C, E] = i1_encode(cfg, G, B, X)

% C: cell per point (coarse assignment)
% E: encoded points (fine assignment)
% G: grid (coarse codebook)
% B: fine codebook
% X: input data points

[R, C] = enc_res(G, X);
E = ex_encode(cfg, B, R);
