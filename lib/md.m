function md(dir)
% make directory without warning if already existing

if exist(dir) ~= 7, mkdir(dir), end
