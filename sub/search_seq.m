function [C, S] = search_seq(I, D, P, A, V, T)
% multi-sequence algorithm (SLOW version): finds cell indices in increasing distance such that total population is at least t, in a block of cell sub-indices I at sub-distances D
%
% C: cell indices found, in increasing distance
% S: increasing cell distances found
% I: cell sub-indices in two subspaces
% D: cell sub-distances in two subspaces
% P: population count per cell (matrix in grid dimensions)
% A: cell indices buffer (unused here)
% V: visited status per cell (unused here)
% T: target total population

c = size(P,1);
[I1 I2] = ndgrid(I(:,1), I(:,2));
[D1 D2] = ndgrid(D(:,1), D(:,2));
[D,i] = sort(D1(:) + D2(:));
I = I1(i) + c * I2(i);
p = cumsum(P(I + 1));  % Matlab offset
f = min(find(p >= T));
if isempty(f), f = length(D); end
I = [I1(i) I2(i)];
C = I(1:f, :);
S = D(1:f);
