function cfg = c2_config(cfg)

cfg = config(cfg);

K = cfg.K;
c = cfg.c;
m = cfg.m;
k = cfg.k;
cfg.cn = uint32(cfg.cn);
cfg.o  = single(cfg.o);

cfg.grid   = sprintf('%sbook/grid_c2_c%d_m%d_k%d.f4', cfg.data, c, m, k);
cfg.book   = sprintf('%sbook/book_c2_c%d_m%d_k%d.f4', cfg.data, c, m, k);
cfg.cell   = sprintf('%scode/cell_c2_c%d_m%d_k%d.%s', cfg.data, c, m, k, cfg.ext(c));
cfg.code   = sprintf('%scode/code_c2_c%d_m%d_k%d.%s', cfg.data, c, m, k, cfg.ext(k));
cfg.pop    = sprintf('%sidx/pop_c2_c%d_m%d_k%d.u4', cfg.data, c, m, k);
cfg.mean_i = sprintf('%sidx/mean_i_c2_c%d_m%d_k%d.u4', cfg.data, c, m, k);
cfg.mean   = sprintf('%sidx/mean_c2_c%d_m%d_k%d.f4', cfg.data, c, m, k);
cfg.idx    = sprintf('%sidx/idx_c2_c%d_m%d_k%d.u4', cfg.data, c, m, k);
cfg.inv    = sprintf('%sidx/inv_c2_c%d_m%d_k%d.mat', cfg.data, c, m, k);
cfg.cen    = sprintf('%sclust/cen_c2_K%d_c%d_m%d_k%d.f4', cfg.data, K, c, m, k);
cfg.asgn   = sprintf('%sclust/asgn_c2_K%d_c%d_m%d_k%d.u4', cfg.data, K, c, m, k);
