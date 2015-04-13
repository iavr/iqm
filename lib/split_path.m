function [d, f, x] = split_path(p)

% d: dir name
% f: file name, without extension if x is requested
% x: extension
% p: full path

i = regexp(p, '/[^/]*$');
d = p(1:i);
f = p(i+1:end);
x = [];

if nargout > 2, [x, f] = file_ext(f); end
