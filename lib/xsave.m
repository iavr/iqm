function xsave(name, a)
% save 1d/2d array, preserving n-dimensional size and with type depending on extension
%
% name: file name
% a: array to save

t = types(ext(name));
if isempty(t), save_ext(name, a, false); return, end

f = fopen(name, 'wb');
if f < 0, error(['Cannot open file ' name ' for writing.']), end
xwrite(f, a, t);
fclose(f);
