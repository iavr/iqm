function Y = dec_sub(B, E, m)

% Y: decoded points (columns)
% B: codebook
% E: input encoded points (columns)
% m: # subspaces

N = size(E, 2);
[d, D] = slices(B, m);
Y = zeros(D, N, 'single');
for i = 1:m
	s = slice(i, d, D);
	Y(s,:) = dec(B(s,:), E(i,:));
end
