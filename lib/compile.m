function compile(name, force)
% compile .cpp file given in path 'name', if binary mex does not exist

if nargin < 2, force = false; end

[dir, file] = split_path(name);
if exist(file) ~= 3 || force
	eval(['mex -silent -outdir ' dir ' ' name '.cpp'])
end
