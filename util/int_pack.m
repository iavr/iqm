function p = int_pack(x, r, c)
% pack rows of integer matrix x into a single row of integers of radix r; each column of x represents the digits of one integer; the output p is of class c

if nargin < 3, c = 'uint64'; end

[k,n] = size(x);
p = zeros(1, n, c);
m = 1;
for i = 1:k
	p = p + m * cast(x(i,:), c);
	m = m * r;
end
