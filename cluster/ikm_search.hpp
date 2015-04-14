//-----------------------------------------------------------------------------
// cell object holding block offset and distance;
// created given block coordinates, which can be retrieved by on()

struct cell
{
	int u;
	float d;
	INLINE cell(int u1, int u2) : u(u1 + w * u2), d(S1[u1] + S2[u2]) {}
	INLINE void on(int& u1, int& u2) { u1 = u % w; u2 = u / w; }

	struct closer
	{
		INLINE bool
		operator()(const cell& c1, const cell& c2) const { return c1.d > c2.d; }
	};

	static int w;
	static float const *S1, *S2;
	static void init(float const *s1, float const *s2, int _w)
		{ S1 = s1; S2 = s2; w = _w; }
};

int cell::w;
float const *cell::S1, *cell::S2;

//-----------------------------------------------------------------------------
// perform nearest neighbor search from centroids to cells, given nearest
// neighbors and distances per subspace.

template<typename V, typename T, typename F>
void search(
	int K, int w, unsigned *p,
	unsigned const *I, float const *S,
	V v, T t, F f
)
{
	typedef priority_queue<cell, vector<cell>, cell::closer> queue;

	// for all centroids k
	for(int k = 0, w2 = 2 * w; k < K; ++k, I += w2, S += w2)
	{
		if(!p[k]) continue;
		cell::init(S, S + w, w);
		v.init(I, I + w);
		f.init(k);
		t.init(k);
		queue q;                                  // queue holding propagation front
		q.push(cell(0, 0));                       // start from top-left cell
		int u1, u2, w1 = w - 1;

		while(!q.empty())
		{
			cell r = q.top();                      // nearest cell from queue
			q.pop();
			r.on(u1, u2);                          // cell coordinates on block
			unsigned i = v.set(u1, u2);            // cell offset on grid
			f(k, i, r.d);                          // operate at cell i, distance r.d
			if(t(i)) break;                        // stop if termination reached
			if(u2 < w1 && (!u1 || v(u1-1, u2+1)))  // if above right is visited ..
				q.push(cell(u1, u2+1));             // .. propagate to the right
			if(u1 < w1 && (!u2 || v(u1+1, u2-1)))  // if below left is visited ..
				q.push(cell(u1+1, u2));             // .. propagate below
		}

		v.clear();
	}
}

//-----------------------------------------------------------------------------
// function object recoding and checking visited status,
// given block coordinates

struct visit
{
	int c, n;
	unsigned *U;
	unsigned char *V;
	unsigned const *I1, *I2;
	visit(unsigned *U, unsigned char *V, int c) : U(U), V(V), c(c) {}
	INLINE void init(unsigned const *i1, unsigned const *i2)
		{ n = -1; I1 = i1; I2 = i2; }
	INLINE unsigned offset(unsigned u1, unsigned u2) { return I1[u1] + c * I2[u2]; }
	INLINE unsigned set(unsigned u1, unsigned u2)
		{ unsigned i = offset(u1, u2); U[++n] = i; V[i] = true; return i; }
	INLINE bool operator()(int u1, int u2) { return V[offset(u1, u2)]; }
	INLINE void clear() { for(; n >= 0; --n) V[U[n]] = false; }
};

//-----------------------------------------------------------------------------
// function object controlling termination

struct term
{
	unsigned t, p;
	unsigned const *P;
	term(unsigned t, unsigned const *P) : t(t), P(P) {}
	INLINE void init(unsigned k) { p = 0; }
	INLINE bool operator()(unsigned i) { p += P[i]; return p >= t; }
};

//-----------------------------------------------------------------------------
// function object recording nearest centroid of each cell

struct cell_nn
{
	unsigned *p, *A;
	float *Z;
	cell_nn(unsigned *p, unsigned *A, float *Z) : p(p), A(A), Z(Z) {}
	INLINE void init(unsigned k) {}
	INLINE void operator()(unsigned k, unsigned i, float d)
	{
		// set centroid k as nearest neighbor of cell i
		// if lower distance found, or assigned centroid is purged
		if(d < Z[i] || !p[A[i]]) { A[i] = k; Z[i] = d; }
	}
};
