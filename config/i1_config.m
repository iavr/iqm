function cfg = i1_config(cfg)

cfg = config(cfg);

c = cfg.c;
m = cfg.m;
k = cfg.k;
r = cfg.r;

if ~cfg.w, cfg.w = cfg.c; end
if ~cfg.t, cfg.t = inf; end

md(sprintf('%sbook',  cfg.data));
md(sprintf('%scode',  cfg.data));
md(sprintf('%sidx',   cfg.data));
md(sprintf('%srank',  cfg.data));

cfg.grid = sprintf('%sbook/grid_c%d_m%d_k%d.f4', cfg.data, c, m, k);
cfg.book = sprintf('%sbook/book_c%d_m%d_k%d.f4', cfg.data, c, m, k);
cfg.cell = sprintf('%scode/cell_c%d_m%d_k%d.%s', cfg.data, c, m, k, cfg.ext(c));
cfg.code = sprintf('%scode/code_c%d_m%d_k%d.%s', cfg.data, c, m, k, cfg.ext(k));
cfg.pop  = sprintf('%sidx/pop_c%d_m%d_k%d.u4', cfg.data, c, m, k);
cfg.idx  = sprintf('%sidx/idx_c%d_m%d_k%d.u4', cfg.data, c, m, k);
cfg.sig  = sprintf('%sidx/sig_c%d_m%d_k%d.%s', cfg.data, c, m, k, cfg.ext(k));
cfg.inv  = sprintf('%sidx/inv_c%d_m%d_k%d.mat', cfg.data, c, m, k);
cfg.rank = sprintf('%srank/rank_c%d_m%d_k%d_r%d.u4', cfg.data, c, m, k, r);
