function [W,A,times] = c2_iter(cfg, W, G, B, E, P, Mi, M, X, C, inputID)

% W:  centroids (target codebook)
% A:  assignment of cells to centroids
% G:  grid (coarse codebook)
% B:  fine codebook (for coarse)
% E:  coarse codebook encoded by fine
% P:  population count per cell (matrix in grid dimensions)
% Mi: for each cell, position of cell mean in sparse matrix M (in grid dimensions)
% M:  data point means per cell (data point dim x # of non-empty cells)
% X:  input data points (optional, for display only)
% C:  cell per point (optional, for display only)

if nargin < 9, X = []; end
if nargin < 11, inputID = []; end

K = cfg.K;                                 % # of centroids
c = cfg.c;                                 % # of cells
w = cfg.w;                                 % search window
null = intmax('uint32');                   % invalid index (value for unassigned)
p = ones (1, K, 'uint32');                 % centroid population
s = zeros(1, K, 'single');                 % centroid variance
A = ones (c, c, 'uint32') * null;          % cell assignment (zero-based)
Z = inf  (c, c, 'single');                 % distance^2 to nearest centroid
U = zeros(w, w, 'uint32');                 % search block buffer
V = zeros(c, c, 'uint8');                  % visited status per cell
Q = ones (c, c, 'uint32') * null;          % quantized centroid per cell
C = int_dec(int_pack(C, c));               % cell position per point
T = uint32(cfg.t * sum(P(:)) / K);         % search target (# of points x N/K)

% timing
times = [];

% main iteration
for n = 1:cfg.it

	% display
	if cfg.verbose
		disp_iter(n, p, P, A+1);
		if cfg.verbose > 1
			if size(X,1) == 2
				disp_2d(n, X, A(C)+1, p, W, s)
			else
				disp_pop(n, p)
			end
			if cfg.verbose > 2, pause, end
		end
	end

	% begin timing
	t = cputime;

	% assignment step - subspace search
	% w nearest cells & distances^2 to centroids per subspace
	if cfg.sub,
		[I, S] = ex_nn_sub(W, B, E, 2, w);
	else
		[I, S] = nn_sub(W, G, 2, w);
	end

	% ikm iteration - assignment by search + update + purge
	ikm_auto(p, W, s, I-1, S, P, Mi-1, M, A, Z, U, V, Q, T, cfg.cn, cfg.o);

	% end timing
	t = cputime - t;
	times = [times t];

	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%%  If inputID is given, save the centers every 5 iterations        %%
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	if (~isempty(inputID) && mod(n,5)==0)
		if(exist('./results') ~= 7), mkdir './results', end
		xsave(sprintf('./results/intermediate_%d_%d_%d.f4', K, inputID, n), W);
	end

	times = [times t];
end
A = A + 1;  % one-based
