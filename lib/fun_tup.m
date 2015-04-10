function varargout = fun_tup(f, varargin)
% Just a shortcut to cellfun, setting 'UniformOutput' to false such that non-uniform cell arrays are output when given arbitrary cell arrays as input.

[varargout{1:nargout}] = cellfun(f, varargin{:}, 'UniformOutput', false);
