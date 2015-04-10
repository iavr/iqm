function dist = ex_search(cfg, Q, B, E)

% (exhaustive) distances
% Q: queries
% B: codebook
% E: encoded points

[d, D] = slices(Q, cfg.m);                % dims, dims / subspace
L = zeros(cfg.k, cfg.m, 'single');        % lookup table

for i = 1:cfg.m                           % subspaces
	s = slice(i, d, D);                    % dimensions in subspace
	L(:,i) = yael_L2sqr(B(s,:), Q(s));     % codeword distances in subspace i
end

dist = search_lu(L, E);
