%--------------------------------
%  cfg.dataset = 'siftsmall';
cfg.dataset = 'sift';
%  cfg.dataset = 'gist';

cfg.c = 32;     % # of cells
cfg.m = 8;      % # of subspaces
cfg.k = 256;    % codebook size
cfg.w = 4;      % search window
cfg.t = 15000;  % search target
cfg.r = 100;    % max recall
cfg.rr = false; % re-rank

%--------------------------------
addpath('../config');
cfg = i2_config(cfg);

%  %--------------------------------
%  fprintf('Learning codebooks\n');
%  X = xload(cfg.learn);
%  u = cputime;
%  [G,B] = i2_learn(cfg, X);
%  fprintf('Learn time: %.3fs\n', cputime - u);
%  xsave(cfg.grid, G);
%  xsave(cfg.book, B);
%  %
%  %--------------------------------
%  fprintf('Encoding vectors\n');
%  G = xload(cfg.grid);
%  B = xload(cfg.book);
%  X = xload(cfg.base);
%  u = cputime;
%  [C,E] = i2_encode(cfg, G, B, X);
%  fprintf('Encode time: %.3fs\n', cputime - u);
%  xsave(cfg.cell, C);
%  xsave(cfg.code, E);
%
%  %--------------------------------
%  fprintf('Inverting index\n');
%  C = xload(cfg.cell);
%  E = xload(cfg.code);
%  u = cputime;
%  [P,I,S,cI,cS] = i2_invert(cfg, C, E);
%  fprintf('Invert time: %.3fs\n', cputime - u);
%  xsave(cfg.pop, P);
%  xsave(cfg.idx, I);
%  xsave(cfg.sig, S);
%  save(cfg.inv, 'cI', 'cS');

%--------------------------------
fprintf('Querying');
if cfg.rr,
	X = xload(cfg.base);
else
	X = [];
end
Q = xload(cfg.query);
G = xload(cfg.grid);
B = xload(cfg.book);
P = xload(cfg.pop);
load(cfg.inv, 'cI', 'cS');
u = cputime;
R = i2_query(cfg, Q, G, B, P, cI, cS, X);
u = cputime - u;
fprintf('Query time: %.3fs total, %.2fms/query\n', u, u * 1000 / size(Q,2));
xsave(cfg.rank, R);

%--------------------------------
fprintf('Evaluating\n');
G = xload(cfg.gt) + 1;
R = xload(cfg.rank);
e = recall(cfg, G, R);
fprintf('Recall@[%s]:\n       [%s]\n', num2str(cfg.R), num2str(e));
