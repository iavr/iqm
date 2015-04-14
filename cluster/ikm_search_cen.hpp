//-----------------------------------------------------------------------------
// structure managing nearest centroids of each centroid.

struct nhood
{
	unsigned n, k;
	vector<float> d;
	vector<unsigned> len, i;
	INLINE nhood(unsigned n, unsigned k) :
		n(n), k(k), len(k), i(n * k), d(n * k) {}
	INLINE unsigned *id(unsigned j) { return &i[j * n]; }
	INLINE float  *dist(unsigned j) { return &d[j * n]; }
	INLINE void clear(unsigned j) { len[j] = 0; }
	INLINE void insert(unsigned j, unsigned i, float d)
	{
		if(len[j] >= n) return;
		id(j)  [len[j]]   = i;
		dist(j)[len[j]++] = d;
	}
};

//-----------------------------------------------------------------------------
// function object recording nearest centroids of each centroid.

struct cen_nn
{
	int K;
	nhood &N;
	unsigned const *Q;
	cen_nn(nhood &N, unsigned const *Q) : K(N.k), N(N), Q(Q) {}
	INLINE void init(unsigned k) { N.clear(k); }
	INLINE void operator()(unsigned k, unsigned i, float d)
	{
		unsigned l = Q[i];
		if(l < K) N.insert(k, l, d);
	}
};

//-----------------------------------------------------------------------------
// quantize each centroid on grid. if two centroids collide, the most
// populated one wins and the other is purged.

void quantize(
	int K, int w, int c, unsigned *p,
	unsigned const *I, unsigned *Q
)
{
	for(int k = 0, w2 = 2 * w; k < K; ++k, I += w2)
	{
		int i = I[0] + c * I[w];
		unsigned l = Q[i];
		if(l >= K) Q[i] = k;
		else if(p[l] < p[k])
		{
			p[l] = 0;
			Q[i] = k;
		}
	}
}

//-----------------------------------------------------------------------------
// clear grid from quantized centroids, so it can be used at next iteration.

void unquantize(
	int K, int w, int c, unsigned const *I, unsigned *Q
)
{
	for(int k = 0, w2 = 2 * w; k < K; ++k, I += w2)
		Q[I[0] + c * I[w]] = K;
}
