function [W,A] = c2_iter(cfg, W, G, B, E, P, M, X, C)

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
A = ones (c, c, 'uint32') * (K+1);                % cell assignment
Z = inf  (c, c, 'single');                        % distance^2 to nearest centroid
U = zeros(w, w, 'uint32');                        % cell indices buffer
V = zeros(c, c, 'uint8');                         % visited status per cell
z = ones (1, K, 'single');                        % centroid variance
f = ones (1, K, 'single');                        % centroid population
F = single(P(:))';                                % cell population
M = reshape(M, [size(M,1) c^2]);                  % cell means, flat over grid
C = int_dec(int_pack(C, c));                      % cell position per point

for n = 1:14                                      % main iteration

	% display
	if cfg.verbose
		disp_iter(n, f, P, A);
		if cfg.verbose > 1
			if size(X,1) == 2
				disp_2d(n, X, A(C), W, z)
			else
				disp_pop(n, f)
			end
			pause
		end
	end

	% assignment step - subspace search
	[IW, SW] = nn_sub(G, W, 2, w);                 % w nearest cells & distances^2 ..
                                                  % .. to centroids per subspace

	% assignment step - multi-sequence
	for k = uint32(find(f > 0))                    % for each centroid
		I = reshape(IW(:,k), [w 2]) - 1;            % top w cells per subspace
		S = reshape(SW(:,k), [w 2]);                % top w distances per subspace
		search_pseq(k, I, S, P, A, Z, U, V, cfg.t); % parallel multi-sequence
	end

	% update step
	[~,I] = sort(A(:));
	[b, e] = range(count(A(:), K+1)');
	for k = 1:K
		i = I(b(k):e(k));
		f(k) = sum(F(i));
		z(k) = sum(F(i) .* Z(i)') / f(k);
		W(:,k) = sum(bsxfun(@times, F(i), M(:,i)), 2) / f(k);
	end

end

A(A == K+1) = 0;  % unassigned cells
