function [P, Mi, M, I, cI] = c2_invert(cfg, C, X)

% P: population count per cell (matrix in grid dimensions)
% Mi: for each cell, position of cell mean in sparse matrix M (in grid dimensions)
% M: data point means per cell (data point dim x # of non-empty cells)
% I: point indices per cell (flat vector)
% cI: point indices per cell (array of vectors, in grid dimensions)
% C: cell per point (coarse assignment)
% X: input data points

c = cfg.c;
C = int_dec(int_pack(C, c));
P = count(C, c^2);
[~,I] = sort(C);

nz = find(P);
l = length(nz);
Mi = zeros(1, c^2, 'uint32');
Mi(nz) = 1:l;

[ca, cz] = range(P);
cI = cell(1, c^2);
for i = 1:c^2
	cI{i} = I(ca(i):cz(i));
end

[ca, cz] = range(P(nz));
D = size(X,1);
M = zeros(D, l, 'single');
for i = 1:l
	M(:,i) = mean(X(:, I(ca(i):cz(i))),2);
end

P = reshape(P, [c c]);
Mi = reshape(Mi, [c c]);
cI = reshape(cI, [c c]);
