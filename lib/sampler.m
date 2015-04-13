function s = sampler(P)
% Sampler from discrete distribution P, using alias method.
% Given sampler s, use s(n) to generate n samples.

[Q, J] = alias_setup(P);
s = @(n) alias_draw(Q, J, rand(1, n), rand(1, n));
