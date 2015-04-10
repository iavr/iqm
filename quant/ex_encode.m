function E = ex_encode(cfg, B, X)

% E: encoded points (assignment)
% B: codebook
% X: input data points

E = enc_sub(B, X, cfg.m);
