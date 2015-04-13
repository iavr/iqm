function a = xload(name, rng)
% load 1d/2d array, preserving n-dimensional size and with type depending on extension
%
% a: array to load
% name: file name
% rng: range of columns to read

if nargin < 2, rng = []; end

[t, s] = type_info(file_ext(name));
if isempty(t), a = load_ext(name, 1, rng, false); return, end

f = fopen(name, 'rb');
if f < 0, error(['Cannot open file ' name ' for reading.']), end
a = xread(f, t, rng, s);
fclose(f);
