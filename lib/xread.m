function a = xread(f, t, rng, sz)
% read 1d/2d array, preserving n-dimensional size
%
% a: array to read
% f: file identifier
% t: type name/conversion, as required by fread's precision argument
% rng: range of columns to read
% sz: type size in bytes

d = fread(f, 1,   'int32');    % # dimensions
r = fread(f, d-1, 'int32')';   % # rows / size in all but last dimension
c = fread(f, 1,   'int32');    % # columns / size in last dimension
p = prod(r);                   % slice size in all but last dimension

% # skip columns
if nargin >= 3 & ~isempty(rng)
	if length(rng) == 1, rng = [rng rng]; end
	[s,l] = deal(rng(1) - 1, rng(2) - rng(1) + 1);
	fseek(f, s * p * sz, 'cof');   % skip columns
	c = min(c - s, l);             % # columns to read
end

if isempty(r), dim = [c 1]; else dim = [r c]; end
a = reshape(fread(f, [p c], ['*' t]), dim);

