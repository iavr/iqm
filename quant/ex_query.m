function [idx, dist] = ex_query(cfg, Q, B, E)

[idx, dist] = ex_nn(B, E, Q, cfg.r, cfg.verbose);
