function [x, n] = ext(n)

% x: extension
% n: file name without extension (output)
% n: file name with extension (input)

i = regexp(n, '\.[^\.]*$');
x = n(i+1:end);
if ~isempty(i), n = n(1:i-1); end
