function [W,A,s,p] = c2_iter(cfg, W, G, B, E, P, M, X, C)

% W: centroids (target codebook)
% A: assignment of cells to centroids
% G: grid (coarse codebook)
% B: fine codebook (for coarse)
% E: coarse codebook encoded by fine
% P: population count per cell (matrix in grid dimensions)
% M: data point means per cell (3D: data point dim x matrix in grid dimensions)
% X: input data points (optional, for display only)
% C: cell per point (optional, for display only)

if nargin < 8, X = []; end

K = cfg.K;                                        % # of centroids
c = cfg.c;                                        % # of cells
w = cfg.w;                                        % search window
p = ones (1, K, 'uint32');                        % centroid population
s = zeros(1, K, 'single');                        % centroid variance
A = ones (c, c, 'uint32') * K;                    % cell assignment; K=unassigned
Z = inf  (c, c, 'single');                        % distance^2 to nearest centroid
U = zeros(w, w, 'uint32');                        % search block buffer
V = zeros(c, c, 'uint8');                         % visited status per cell
C = int_dec(int_pack(C, c));                      % cell position per point

% main iteration
for n = 1:14

	% display
	if cfg.verbose
		disp_iter(n, p, P, A);
		if cfg.verbose > 1
			if size(X,1) == 2
				disp_2d(n, X, A(C), W, s)
			else
				disp_pop(n, p)
			end
			pause
		end
	end

	% assignment step - subspace search
	[I, S] = nn_sub(G, W, 2, w);                   % w nearest cells & distances^2 ..
                                                  % .. to centroids per subspace

	% ikm iteration - assignment by search + update
	ikm_iter(p, W, s, I-1, S, P, M, A, Z, U, V, cfg.t);

end
