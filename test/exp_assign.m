function I = exp_assign(C, X)
	dist = yael_L2sqr(C, X);
	[m I] = min(dist);	

