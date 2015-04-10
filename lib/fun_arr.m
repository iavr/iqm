function varargout = fun_arr(f, varargin)
% Just a shortcut to arrayfun, setting 'UniformOutput' to false such that non-uniform cell arrays are output when given arbitrary arrays as input.

[varargout{1:nargout}] = arrayfun(f, varargin{:}, 'UniformOutput', false);
