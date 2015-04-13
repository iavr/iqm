addpath('../config');

%--------------------------------
cfg.m = 8;     % # of subspaces
cfg.k = 256;   % codebook size
cfg.r = 1000;  % max recall

%  dataset = 'siftsmall';
cfg.dataset = 'sift';
cfg = ex_config(cfg);

%  %--------------------------------
%  fprintf('Learning codebooks\n');
%  X = xload(cfg.learn);
%  u = cputime;
%  B = ex_learn(cfg, X);
%  fprintf('Learn time: %.3fs\n', cputime - u);
%  xsave(cfg.book, B);
%
%  %--------------------------------
%  fprintf('Encoding vectors\n');
%  B = xload(cfg.book);
%  X = xload(cfg.base);
%  u = cputime;
%  E = ex_encode(cfg, B, X);
%  fprintf('Encode time: %.3fs\n', cputime - u);
%  xsave(cfg.code, E);

%--------------------------------
fprintf('Querying');
B = xload(cfg.book);
E = xload(cfg.code);
Q = xload(cfg.query);
u = cputime;
R = ex_query(cfg, Q, B, E);
u = cputime - u;
fprintf('Query time: %.3fs total, %.2fms/query\n', u, u * 1000 / size(Q,2));
xsave(cfg.rank, R);

%--------------------------------
fprintf('Evaluating\n');
G = xload(cfg.gt) + 1;
R = xload(cfg.rank);
e = recall(cfg, G, R);
fprintf('Recall@[%s]:\n       [%s]\n', num2str(cfg.R), num2str(e));
