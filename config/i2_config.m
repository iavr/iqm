function cfg = i2_config(cfg)

cfg = config(cfg);

c = cfg.c;
m = cfg.m;
k = cfg.k;
r = cfg.r;
cfg.t = uint32(cfg.t);

if ~cfg.w, cfg.w = cfg.c; end

cfg.grid = sprintf('%sbook/grid_i2_c%d_m%d_k%d.f4', cfg.data, c, m, k);
cfg.book = sprintf('%sbook/book_i2_c%d_m%d_k%d.f4', cfg.data, c, m, k);
cfg.cell = sprintf('%scode/cell_i2_c%d_m%d_k%d.%s', cfg.data, c, m, k, cfg.ext(c));
cfg.code = sprintf('%scode/code_i2_c%d_m%d_k%d.%s', cfg.data, c, m, k, cfg.ext(k));
cfg.pop  = sprintf('%sidx/pop_i2_c%d_m%d_k%d.u4', cfg.data, c, m, k);
cfg.idx  = sprintf('%sidx/idx_i2_c%d_m%d_k%d.u4', cfg.data, c, m, k);
cfg.sig  = sprintf('%sidx/sig_i2_c%d_m%d_k%d.%s', cfg.data, c, m, k, cfg.ext(k));
cfg.inv  = sprintf('%sidx/inv_i2_c%d_m%d_k%d.mat', cfg.data, c, m, k);
cfg.rank = sprintf('%srank/rank_i2_c%d_m%d_k%d_r%d.u4', cfg.data, c, m, k, r);
