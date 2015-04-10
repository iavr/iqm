function [idx, dist] = ex_query(cfg, Q, B, E)

% idx: top-ranking points
% dist: top-ranking (least) distances
% Q: queries
% B: codebook
% E: encoded points

nq = size(Q,2);                                % dimensions, # queries
r = cfg.r;                                     % # top ranking results

dist = zeros(r, nq, 'single');                 % top ranking distances
idx  = zeros(r, nq, 'uint32');                 % top ranking indices

for q = 1:nq                                   % queries
	ex = ex_search(cfg, Q(:,q), B, E);          % exhaustive search
	[dist(:,q), idx(:,q)] = yael_kmin(ex, r);   % top r points per query
	if cfg.verbose & mod(q,100)==0, fprintf('.'), end
end

if cfg.verbose, fprintf('\n'), end
