function [y, i] = sortcols(x, c)
% the analogous of sortrows, which is missing; see sortrows for help

if nargin < 2, c = 1:size(x,1); end

[y, i] = sortrows(x', c);
y = y';
i = i';
