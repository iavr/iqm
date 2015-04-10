function [R, C] = enc_res(G, X)

% R: residue
% C: encoding (coarse assignment)
% G: grid (coarse codebook)
% X: input data points

C = enc(G, X);
R = X - dec(G, C);
