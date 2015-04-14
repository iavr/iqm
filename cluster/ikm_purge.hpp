//-----------------------------------------------------------------------------
// order cells by decreasing population

struct popular
{
	unsigned const *p;
	popular(unsigned const *p) : p(p) {}
	INLINE bool
	operator()(unsigned i, unsigned j) const { return p[i] > p[j]; }
};

//-----------------------------------------------------------------------------
// purge step

void purge(int K, int D, unsigned *p, float const *s, nhood &N, float o)
{
	vector<float> e(N.n);
	vector<unsigned> rk(K), ord(K);
	iota(ord.begin(), ord.end(), 0);
	std::sort(ord.begin(), ord.end(), popular(p));
	for(int k = 0; k < K; ++k)
		rk[ord[k]] = k;
	for(int k, q = 0; q < K; ++q)
	{
		if(!p[k = ord[q]]) continue;
		unsigned j, l = N.len[k], *i = N.id(k);
		float t, m = inf(), sk = s[k], *d = N.dist(k);
		for(int n = 0; n < l; ++n)
		{
			if(!p[j = i[n]] || rk[j] > rk[k]) continue;
			float s2 = sk + s[j];
			t = e[n] = d[n] / s2 + D * std::log(s2);
			if(t < m) m = t;
		}
		t = 0;
		for(int n = 0; n < l; ++n)
		{
			if(!p[j = i[n]] || rk[j] > rk[k]) continue;
			t += e[n] = p[j] * std::exp((m - e[n]) / 2);
		}
		if(e[0] < std::pow(o, D/2) * t) p[k] = 0;
	}
}
