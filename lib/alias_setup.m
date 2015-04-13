function [Q, J] = alias_setup(P)
% Alias method - O(K) table setup. Called by sampler.m.
% This is a SLOW implementation; use the mex file instead.
% Also see mex file for comments and reference.

P = double(P(:))';
K = length(P);
Q = K * P / sum(P);
J = zeros(1, K);
small = find(Q < 1);
large = find(Q >= 1);
T = [small large];
il = length(small);
ik = il + 1;
while il > 0 && ik <= K
	l = T(il);
	k = T(ik);
	J(l) = k - 1;               % zero-based index
	Q(k) = Q(k) + Q(l) - 1;
	if Q(k) < 1
		T(il) = k;
		ik = ik + 1;
	else
		il = il - 1;
	end
end
