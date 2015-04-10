function x = int_pack(p, r, k, c)
% unpack single row of integers of radix r into k rows of integer matrix x, of class c, where each column represents a k-digit integer

if nargin < 4, c = 'uint64'; end

x = zeros(k, length(p), c);
for i = 1:k
	x(i,:) = mod(p, r);
	p = floor(p / r);
end
