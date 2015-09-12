function [idx, dist] = i1_query(cfg, Q, G, B, P, cI, cS, X)

% idx: top-ranking points
% dist: top-ranking (least) distances
% Q: query points
% G: grid (coarse codebook)
% B: fine codebook
% P: population count per cell
% cI: point indices per cell (array of vectors)
% cS: encoded points per cell (array of matrices)
% X: input data points (optional for re-ranking)

if nargin < 7, X = []; end;

[d, D, N] = slices(Q, cfg.m);                  % dims / subspace, dims, # queries
L = zeros(cfg.k, cfg.m, 'single');             % lookup table
r = cfg.r;                                     % # top ranking results
dist = inf  (r, N, 'single');                  % top ranking distances
idx  = zeros(r, N, 'uint32');                  % top ranking indices
C = nn(Q, G, cfg.w);                           % w nearest cells to queries

for q = 1:N                                    % queries
	w = cfg.w;                                  % # of top cells
	c = C(:,q);                                 % top w cells
	f = min(find(cumsum(P(c)) >= cfg.t));       % top cells containing ..
	if ~isempty(f), w = min(w, f); end          % .. up to t vectors
	R = res(G, c(1:w), Q(:,q));                 % query residuals
	cd = [];                                    % cell distances
	ci = [];                                    % cell indices
	for i = 1:w
		for j = 1:cfg.m                          % subspaces
			s = (j-1) * d + (1:d);                % dimensions in subspace
			L(:,j) = yael_L2sqr(B(s,:), R(s,i));  % codeword distances in subspace i
		end
		cd = [cd; search_lu(L, cS{c(i)})];       % cell search: distances
		ci = [ci; cI{c(i)}'];                    % cell search: indices
	end
	T = 1:min(r, length(cd));
	if isempty(X)
		[dist(T,q), i] = kmin(cd, r);            % top r distances per query
		idx(T,q) = ci(i);                        % top r indices per query
	else
		[~,i] = kmin(cd, r);                     % top r distances per query
		ci = ci(i);                              % top r indices per query
		[i, dist(T,q)] = nn(Q(:,q), X(:,ci), r); % re-ranking on X
		idx(T,q) = ci(i);
	end
	if cfg.verbose & mod(q,100)==0, fprintf('.'), end
end

if cfg.verbose, fprintf('\n'), end
