function [P, Mi, M, I, cI] = c2_invert(cfg, C, X)

% P: population count per cell (matrix in grid dimensions)
% Mi: for each cell, position of cell mean in sparse matrix M
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
Mi = zeros(1, c^2, 'uint32');
Mi(nz) = 1:length(nz);

[ca, cz] = range(P);
D = size(X,1);
M = zeros(D, c^2, 'single');
cI = cell(1, c^2);
for i = 1:c^2
	cI{i} = I(ca(i):cz(i));
	if P(i), M(:,Mi(i)) = mean(X(:, cI{i}),2); end
end

P = reshape(P, [c c]);
M = reshape(M, [D c c]);
cI = reshape(cI, [c c]);
