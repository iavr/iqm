function [idx, dist] = ex_query(cfg, Q, B, E)

[idx, dist] = ex_nn(Q, B, E, cfg.r, cfg.verbose);
