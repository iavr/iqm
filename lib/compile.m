function compile(name, force)
% compile .cpp file given in path 'name', if binary mex does not exist

if nargin < 2, force = false; end

[dir, file] = splitpath(name);
if exist(file) ~= 3 || force
	eval(['mex -outdir ' dir ' ' name '.cpp'])
end
