function B = ex_learn(cfg, X)

% B: codebook
% X: input data points

B = learn_sub(cfg, X, cfg.m, cfg.k);
