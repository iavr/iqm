addpath('../config');

%--------------------------------
cfg.c = 256;    % # of cells
cfg.m = 8;      % # of subspaces
cfg.k = 256;    % codebook size
cfg.w = 16;      % search window
cfg.t = 0;      % search target
cfg.r = 1000;    % max recall
cfg.rr = true;  % re-rank

%  dataset = 'siftsmall';
cfg.dataset = 'sift';
cfg = i1_config(cfg);

%  %--------------------------------
%  fprintf('Learning codebooks\n');
%  X = xload(cfg.learn);
%  u = cputime;
%  [G,B] = i1_learn(cfg, X);
%  fprintf('Learn time: %.3fs\n', cputime - u);
%  xsave(cfg.grid, G);
%  xsave(cfg.book, B);
% 
%  %--------------------------------
%  fprintf('Encoding vectors\n');
%  G = xload(cfg.grid);
%  B = xload(cfg.book);
%  X = xload(cfg.base);
%  u = cputime;
%  [C,E] = i1_encode(cfg, G, B, X);
%  fprintf('Encode time: %.3fs\n', cputime - u);
%  xsave(cfg.cell, C);
%  xsave(cfg.code, E);

%--------------------------------
fprintf('Inverting index\n');
C = xload(cfg.cell);
E = xload(cfg.code);
u = cputime;
[P,I,S,cI,cS] = i1_invert(cfg, C, E);
fprintf('Invert time: %.3fs\n', cputime - u);
xsave(cfg.pop, P);
xsave(cfg.idx, I);
xsave(cfg.sig, S);
save(cfg.inv, 'cI', 'cS');

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
R = i1_query(cfg, Q, G, B, P, cI, cS, X);
u = cputime - u;
fprintf('Query time: %.3fs total, %.2fms/query\n', u, u * 1000 / size(Q,2));
xsave(cfg.rank, R);

%--------------------------------
fprintf('Evaluating\n');
G = xload(cfg.gt) + 1;
R = xload(cfg.rank);
e = recall(cfg, G, R);
fprintf('Recall@[%s]:\n       [%s]\n', num2str(cfg.R), num2str(e));
