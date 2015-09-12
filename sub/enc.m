function E = enc(B, X)

% E: encoded points (columns)
% B: codebook
% X: input data points (columns)

type = int_type(size(B,2));
E = int_enc(nn(X, B), type);
