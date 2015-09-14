function cfg = config(cfg)

%-------------------------------------------
% adjust to your local settings
%-------------------------------------------
global yael xio home
local
%-------------------------------------------

% verbose operation?
if ~isfield(cfg, 'verbose')
	cfg.verbose = 1;
end

% matlab path
addpath(yael, xio);
addpath('../lib', '../util', '../disp', '../config');
addpath('../sub', '../quant', '../cluster');

% compile mex files
compile('../lib/alias_setup')
compile('../lib/alias_draw')
compile('../sub/search_lu')
compile('../sub/search_seq')
compile('../cluster/ikm_base')
compile('../cluster/ikm_auto', true)

% dataset location
cfg.home  = home;
cfg.data  = [cfg.home cfg.dataset '/'];
cfg.raw   = [cfg.data 'raw/'];

% dataset choices
switch cfg.dataset
case {'2d_uni','2d_gm'}
	cfg.synth = true;
	md(cfg.data)
	md(cfg.raw)
	cfg.learn = [cfg.raw cfg.dataset '_learn.f4'];
	cfg.base  = [cfg.raw cfg.dataset '_base.f4'];
otherwise
	cfg.synth = false;
	cfg.learn = [cfg.raw cfg.dataset '_learn.fvecs'];
	cfg.base  = [cfg.raw cfg.dataset '_base.fvecs'];
	cfg.query = [cfg.raw cfg.dataset '_query.fvecs'];
	cfg.gt    = [cfg.raw cfg.dataset '_groundtruth.ivecs'];
end

% generate (and save) dataset?
if isfield(cfg, 'gen') && cfg.gen
	cfg = gen(cfg);
end

% output folders
md([cfg.data 'book/'])
md([cfg.data 'code/'])
md([cfg.data 'idx/'])
md([cfg.data 'rank/'])
md([cfg.data 'clust/'])

% functions
cfg.ext = @(k) int_type(k);
cfg.cluster = @(X,k) yael_kmeans(X, k, 'niter', 20, 'verbose', false);

% recall@R evaluation
if isfield(cfg, 'r')
	cfg.R = [1 2 5 10 20 50 100 200 500 1000 2000 5000 10000];
	cfg.R = cfg.R(cfg.r >= cfg.R);
end

end
