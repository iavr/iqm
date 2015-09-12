function [C, E] = c2_encode(cfg, G, B, X)

% C: cell per point (coarse assignment)
% E: coarse codebook encoded by fine
% G: grid (coarse codebook)
% B: fine codebook (for coarse)
% X: input data points

C = enc_sub(G, X, 2);
if cfg.sub,
	E = ex_encode(cfg, B, G);
else
	E = [];
end
