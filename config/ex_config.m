function cfg = ex_config(cfg)

cfg = config(cfg);

m = cfg.m;
k = cfg.k;
r = cfg.r;

md(sprintf('%sbook',  cfg.data));
md(sprintf('%scode',  cfg.data));
md(sprintf('%srank',  cfg.data));

cfg.book = sprintf('%sbook/book_m%d_k%d.f4', cfg.data, m, k);
cfg.code = sprintf('%scode/code_m%d_k%d.%s', cfg.data, m, k, cfg.ext(k));
cfg.rank = sprintf('%srank/rank_m%d_k%d_r%d.u4', cfg.data, m, k, r);
