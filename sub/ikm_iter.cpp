#include <queue>
#include "mex.h"

#define INLINE __attribute__((always_inline))

using std::vector;
using std::priority_queue;

//-----------------------------------------------------------------------------

// cell object holding block offset and distance;
// created given block coordinates, which can be retrieved by on()
struct cell
{
	int u;
	float d;
	static int w;
	static float const *S1, *S2;
	INLINE cell(int u1, int u2) : u(u1 + w * u2), d(S1[u1] + S2[u2]) {}
	INLINE void on(int& u1, int& u2) { u1 = u % w; u2 = u / w; }
};

int cell::w;
float const *cell::S1, *cell::S2;

// function object comparing cell to order by increasing distance
struct gt
{
	INLINE bool
	operator()(const cell& c1, const cell& c2) { return c1.d > c2.d; }
};

// function object checking visited status, given block coordinates
struct visit
{
	unsigned const *I1, *I2;
	unsigned char *V;
	int c;
	visit(unsigned const *I1, unsigned const *I2, unsigned char *V, int c) :
		I1(I1), I2(I2), V(V), c(c) {}
	INLINE bool
	operator()(int u1, int u2) { return V[I1[u1] + c * I2[u2]]; }
};

//-----------------------------------------------------------------------------
// assign step. performs nearest neighbor queries from centroids to cells
// (given nearest neighbors and distances per subspace), and checks such that
// nearest centroids are stored per cell. some cells may remain unassigned.

void assign(
	int K, int w, int c, unsigned *p,
	unsigned const *I, float const *S, unsigned const *P,
	unsigned *A, float *Z, unsigned *U, unsigned char *V, unsigned t
)
{
	// for all centroids k
	for(int k = 0, w2 = 2 * w; k < K; ++k, I += w2, S += w2)
	{
		if(!p[k]) continue;
		unsigned const *I1 = I, *I2 = I + w;      // separate nearest sub-indices ..
		float    const *S1 = S, *S2 = S + w;      // .. and distances per subspace
		cell::w = w;
		cell::S1 = S1;
		cell::S2 = S2;
		visit v(I1, I2, V, c);
		priority_queue<cell, vector<cell>, gt> q; // queue holding propagation front
		q.push(cell(0, 0));                       // start from top-left cell
		unsigned i, i1, i2, p = 0;
		int u1, u2, w1 = w - 1, n = -1;

		while(!q.empty())
		{
			cell r = q.top();                      // nearest cell from queue
			q.pop();
			r.on(u1, u2);                          // cell coordinates on block
			i1 = I1[u1];                           // cell coordinates on grid
			i2 = I2[u2];
			i = i1 + c * i2;                       // cell offset on grid
			U[++n] = i;                            // record offset
			V[i] = true;                           // set as visited
			p += P[i];                             // add to population
			if(p >= t) break;                      // stop if target population reached
			if(u2 < w1 && (!u1 || v(u1-1, u2+1)))  // if above right is visited ..
				q.push(cell(u1, u2+1));             // .. propagate to the right
			if(u1 < w1 && (!u2 || v(u1+1, u2-1)))  // if below left is visited ..
				q.push(cell(u1+1, u2));             // .. propagate below
			if(r.d < Z[i])                         // set k as nearest neighbor ..
				{ A[i] = k; Z[i] = r.d; }           // .. of cell i
		}

		for(; n >= 0; --n)                        // backtrack to recorded offsets ..
			V[U[n]] = false;                       // and clear visited status
	}
}

//-----------------------------------------------------------------------------
// update step. all cells are scanned and their contribution is collected to
// centroid population, weighted variance, and weighted mean (vector),
// for the centroid to which they are assigned. populations are used as weights.

void update(
	int K, int D, int c, unsigned *p, float *W, float *s,
	unsigned const *P, float const *M, unsigned const *A, float const *Z
)
{
	std::fill(p, p + K, 0u);           // initialize population, variance and
	std::fill(s, s + K, .0f);          // mean to zero for each centroid
	std::fill(W, W + D * K, .0f);

	// for all cells i
	for(int i = 0, g = c * c; i < g; ++i, M += D)
	{
		int k = A[i];                   // assigned centroid
		float q = P[i];                 // cell population
		if(!q || k == K) continue;      // skip unassigned or empty cells
		p[k] += P[i];                   // contribute to centroid population
		s[k] += q * Z[i];               // contribute to centroid (weighted) variance
		float *C = W + D * k;
		for(int d = 0; d < D; ++d)      // contribute to centroid (weighted) mean
			C[d] += q * M[d];
	}

	// for all centroids k
	for(int k = 0; k < K; ++k, W += D)
	{
		float q = p[k];                 // centroid population
		if(!q) continue;                // skip empty centroids
		s[k] /= q;                      // normalize variance by total population
		for(int d = 0; d < D; ++d)      // normalize mean by total population
			W[d] /= q;
	}
}

//-----------------------------------------------------------------------------

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, mxArray const *prhs[])
{
	if (nrhs != 12) mexErrMsgTxt("Needs 12 input arguments");
	if (nlhs != 0)  mexErrMsgTxt("Needs 0 output argument");

	mxArray const *ap = prhs[0],  // 1 x K     centroid populations
	              *aW = prhs[1],  // D x K     centroids (means)
	              *as = prhs[2],  // 1 x K     centroid variances
	              *aI = prhs[3],  // 2w x K    cell sub-indices
	              *aS = prhs[4],  // 2w x K    sub-distances
	              *aP = prhs[5],  // c x c     cell populations
	              *aM = prhs[6],  // D x c^2   cell means
	              *aA = prhs[7],  // c x c     cell assignment
	              *aZ = prhs[8],  // c x c     distance^2 to centroid
	              *aU = prhs[9],  // w x w     search block buffer
	              *aV = prhs[10], // c x c     visited cells
	              *aT = prhs[11]; // scalar    target population

	int K  = mxGetN(ap),          // # of centroids
	    D  = mxGetM(aW),          // # of dimensions
	    w2 = mxGetM(aI),          // block size x 2
	    w  = w2 / 2,              // block size (w x w)
	    c  = mxGetM(aP);          // grid size  (c x c)

	if(mxGetM(ap) != 1)
		mexErrMsgTxt("Dimensions of argument 1 invalid.");

	if(mxGetN(aW) != K)
		mexErrMsgTxt("Dimensions of arguments 1,2 not consistent.");

	if(mxGetM(as) != 1 || mxGetN(as) != K)
		mexErrMsgTxt("Dimensions of arguments 1,3 not consistent.");

	if(w2 % 2)
		mexErrMsgTxt("Dimensions of argument 4 invalid.");

	if(mxGetN(aI) != K)
		mexErrMsgTxt("Dimensions of arguments 1,4 not consistent.");

	if(mxGetM(aS) != w2 || mxGetN(aS) != K)
		mexErrMsgTxt("Dimensions of arguments 4,5 not consistent.");

	if(mxGetN(aP) != c)
		mexErrMsgTxt("Dimensions of argument 6 not consistent.");

	if(mxGetM(aM) != D || mxGetN(aM) != c*c)
		mexErrMsgTxt("Dimensions of argument 7 invalid.");

	if(mxGetM(aA) != c || mxGetN(aA) != c)
		mexErrMsgTxt("Dimensions of argument 8 invalid.");

	if(mxGetM(aZ) != c || mxGetN(aZ) != c)
		mexErrMsgTxt("Dimensions of argument 9 invalid.");

	if(mxGetM(aU) != w || mxGetN(aU) != w)
		mexErrMsgTxt("Dimensions of argument 10 invalid.");

	if(mxGetM(aV) != c || mxGetN(aV) != c)
		mexErrMsgTxt("Dimensions of argument 11 invalid.");

	if(mxGetClassID(ap) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 1 should be of type uint32.");

	if(mxGetClassID(aI) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 2 should be of type uint32.");

	if(mxGetClassID(aW) != mxSINGLE_CLASS)
		mexErrMsgTxt("Argument 3 should be of type single.");

	if(mxGetClassID(as) != mxSINGLE_CLASS)
		mexErrMsgTxt("Argument 4 should be of type single.");

	if(mxGetClassID(aS) != mxSINGLE_CLASS)
		mexErrMsgTxt("Argument 5 should be of type single.");

	if(mxGetClassID(aP) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 6 should be of type uint32.");

	if(mxGetClassID(aM) != mxSINGLE_CLASS)
		mexErrMsgTxt("Argument 7 should be of type single.");

	if(mxGetClassID(aA) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 8 should be of type uint32.");

	if(mxGetClassID(aZ) != mxSINGLE_CLASS)
		mexErrMsgTxt("Argument 9 should be of type single.");

	if(mxGetClassID(aU) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 10 should be of type uint32.");

	if(mxGetClassID(aV) != mxUINT8_CLASS)
		mexErrMsgTxt("Argument 11 should be of type uint8.");

	if(mxGetClassID(aT) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 12 should be of type uint32.");

//-----------------------------------------------------------------------------

	unsigned      *p = (unsigned*)      mxGetPr(ap);      // centroid populations
	float         *W = (float*)         mxGetPr(aW);      // centroids (means)
	float         *s = (float*)         mxGetPr(as);      // centroid variances
	unsigned      *I = (unsigned*)      mxGetPr(aI);      // cell sub-indices
	float         *S = (float*)         mxGetPr(aS);      // sub-distances
	unsigned      *P = (unsigned*)      mxGetPr(aP);      // cell populations
	float         *M = (float*)         mxGetPr(aM);      // cell means
	unsigned      *A = (unsigned*)      mxGetPr(aA);      // cell assignment
	float         *Z = (float*)         mxGetPr(aZ);      // distance^2 to centroid
	unsigned      *U = (unsigned*)      mxGetPr(aU);      // search block buffer
	unsigned char *V = (unsigned char*) mxGetPr(aV);      // visited cells
	unsigned       t = (unsigned)       mxGetScalar(aT);  // target population

	assign(K, w, c, p, I, S, P, A, Z, U, V, t);
	update(K, D, c, p, W, s, P, M, A, Z);
}
