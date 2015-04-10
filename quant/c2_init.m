function W = c2_init(cfg, G, B, E, P)

% W: centroids (target codebook)
% G: grid (coarse codebook)
% B: fine codebook (for coarse)
% E: coarse codebook encoded by fine
% P: population count per cell (matrix in grid dimensions)

s = sampler(P);
I = int_enc(s(cfg.K));
E = int_unpack(I, cfg.c, 2);
W = dec_sub(G, E, 2);

