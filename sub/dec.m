function Y = dec(B, E)

% Y: decoded points (columns)
% B: codebook
% E: input encoded points (columns)

Y = B(:, int_dec(E));
