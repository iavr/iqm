function [b,e] = range(s)

% b: begin positions of bins/cells/slices (starting at one)
% e: end positions of bins/cells/slices (starting at one)
% s: bin/cell/slice sizes (counts) (row vector)

e = cumsum(s, 2);         % end position
b = [0 e(1:end-1)] + 1;   % begin position
