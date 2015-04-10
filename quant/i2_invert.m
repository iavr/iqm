function [P, I, S, cI, cS] = i2_invert(cfg, C, E)

% P: population count per cell (matrix in grid dimensions)
% I: point indices per cell (flat vector)
% S: encoded points per cell (flat matrix)
% cI: point indices per cell (array of vectors, in grid dimensions)
% cS: encoded points per cell (array of matrices, in grid dimensions)
% C: cell per point (coarse assignment)
% E: encoded points (fine assignment)

c = cfg.c;
C = int_dec(int_pack(C, c));
P = count(C, c^2);
[~,I] = sort(C);
S = E(:,I);

cI = cell(1, c^2);
cS = cell(1, c^2);
[ca, cz] = range(P);
for i = 1:c^2
	cI{i} = I(ca(i):cz(i));
	cS{i} = S(:, ca(i):cz(i));
end

P = reshape(P, [c c]);
cI = reshape(cI, [c c]);
cS = reshape(cS, [c c]);
