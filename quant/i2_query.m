function [idx, dist] = i2_query(cfg, Q, G, B, P, cI, cS, X)

% idx: top-ranking points
% dist: top-ranking (least) distances
% Q: queries
% G: grid (coarse codebook)
% B: fine codebook
% P: population count per cell
% cI: point indices per cell (array of vectors)
% cS: encoded points per cell (array of matrices)
% X: input data points (optional for re-ranking)

if nargin < 7, X = []; end;

[d, D, nq] = slices(Q, cfg.m);                 % dims, dims / subspace, # queries
L = zeros(cfg.k, cfg.m, 'single');             % lookup table
r = cfg.r;                                     % # top ranking results
dist = inf  (r, nq, 'single');                 % top ranking distances
idx  = zeros(r, nq, 'uint32');                 % top ranking indices
U = zeros(cfg.w, cfg.w, 'uint32');             % cell indices buffer
V = zeros(cfg.c, cfg.c, 'uint8');              % visited status per cell
[IQ, SQ] = nn_sub(G, Q, 2, cfg.w);             % w nearest cells & distances^2 ..
                                               % .. to queries per subspace
for q = 1:nq                                   % queries
	I = reshape(IQ(:,q), [cfg.w 2]) - 1;        % top w cells per subspace
	S = reshape(SQ(:,q), [cfg.w 2]);            % top w distances per subspace
	C = search_seq(I, S, P, U, V, cfg.t) + 1;   % multi-sequence algorithm
	R = res_sub(G, C, Q(:,q), 2);               % query residuals
	cd = [];                                    % cell distances
	ci = [];                                    % cell indices
	for i = 1:size(C,1)
		for j = 1:cfg.m                          % subspaces
			s = (j-1) * d + (1:d);                % dimensions in subspace
			L(:,j) = yael_L2sqr(B(s,:), R(s,i));  % codeword distances in subspace i
		end
		z = num2cell(C(i,:));                    % cell coordinates on grid
		cd = [cd; search_lu(L, cS{z{:}})];       % cell search: distances
		ci = [ci; cI{z{:}}'];                    % cell search: indices
	end
	T = 1:min(r, length(cd));
	if isempty(X)
		[dist(T,q), i] = kmin(cd, r);            % top r distances per query
		idx(T,q) = ci(i);                        % top r indices per query
	else
		[~,i] = kmin(cd, r);                     % top r distances per query
		ci = ci(i);                              % top r indices per query
		[i, dist(T,q)] = nn(X(:,ci), Q(:,q), r); % re-ranking on X
		idx(T,q) = ci(i);
	end
	if cfg.verbose & mod(q,100)==0, fprintf('.'), end
end

if cfg.verbose, fprintf('\n'), end
