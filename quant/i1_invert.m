function [P, I, S, cI, cS] = i1_invert(cfg, C, E)

% P: population count per cell
% I: point indices per cell (flat vector)
% S: encoded points per cell (flat matrix)
% cI: point indices per cell (array of vectors)
% cS: encoded points per cell (array of matrices)
% C: cell per point (coarse assignment)
% E: encoded points (fine assignment)

c = cfg.c;
C = int_dec(C);
P = count(C, c);
[~,I] = sort(C);
S = E(:,I);

cI = cell(1, c);
cS = cell(1, c);
[ca, cz] = range(P);
for i = 1:c
	cI{i} = I(ca(i):cz(i));
	cS{i} = S(:, ca(i):cz(i));
end
