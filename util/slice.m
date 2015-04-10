function s = slice(i,d,D)

% s: slice (range)
% i: number of slice (counter)
% d: slice size (length)
% D: total length

s = (i-1) * d + (1:d);
s(s > D) = [];

