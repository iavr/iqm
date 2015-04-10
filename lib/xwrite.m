function xwrite(f, a, t)
% write 1d/2d array, preserving n-dimensional size
%
% f: file identifier
% a: array to write
% t: type name/conversion, as required by fwrite's precision argument

% drop last dimension if column vector
d = ndims(a);
s = size(a);
if s(end) == 1, d = d-1; s = s(1:end-1); end

% write
fwrite(f, d, 'int32');
fwrite(f, s, 'int32');
fwrite(f, a, t);
