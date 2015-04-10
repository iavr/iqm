function E = int_enc(X, type)

% E: encoded data
% X: input data
% type: encoding data type

if nargin < 2, type = 'uint64'; end

E = cast(X - 1, type);
