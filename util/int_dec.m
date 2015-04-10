function Y = int_dec(E, type)

% Y: decoded data
% E: input encoded data
% type: decoded data type

if nargin < 2, type = 'uint64'; end

Y = cast(E, type) + 1;
