function x = alias_draw(Q, J, R, V)
% Alias method - O(N) sampling. Called by sampler.m.
% This is a SLOW implementation; use the mex file instead.
% Also see mex file for comments and reference.

x = floor(R * length(Q));
i = find(V > Q(x + 1));
x(i) = J(x(i) + 1);          % zero-based indexing (0..K-1)
