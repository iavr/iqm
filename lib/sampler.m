function s = sampler(p)
% sampler from discrete distribution p, using alias method
% given sampler s, use s(n) to generate n samples

p = double(p(:))';
k = length(p);
U = k * p / sum(p);
B = zeros(1, k);
small = find(U < 1);
large = find(U >= 1);
si = length(small);
li = length(large);
while min(si, li) > 0
	se = small(si);
	le = large(li);
	B(se) = le;
	U(le) = U(le) + U(se) - 1;
	if U(le) < 1
		small(si) = le;
		li = li - 1;
	else
		si = si - 1;
	end
end

%--------------------------------
function x = sample(n)
	x = floor(rand(1, n) * k) + 1;
	i = find(rand(1, n) >= U(x));
	x(i) = B(x(i));
end
%--------------------------------

s = @sample;

end
