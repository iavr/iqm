#include <stdio.h>
#include <queue>
#include "mex.h"

#define INLINE __attribute__((always_inline))

using std::vector;
using std::priority_queue;

//-----------------------------------------------------------------------------

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

struct gt
{
	INLINE bool
	operator()(const cell& c1, const cell& c2) { return c1.d > c2.d; }
};

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

void search_seq(
	unsigned const *I, float const *S, unsigned const *P,
	unsigned *U, unsigned char *V,
	unsigned t, int w, int c, int& n
)
{
	unsigned const *I1 = I, *I2 = I + w;
	float    const *S1 = S, *S2 = S + w;
	cell::w = w;
	cell::S1 = S1;
	cell::S2 = S2;
	visit v(I1, I2, V, c);
	priority_queue<cell, vector<cell>, gt> q;
	q.push(cell(0, 0));
	unsigned i, i1, i2, p = 0;
	int u1, u2, w1 = w - 1;
	n = 0;

	while(!q.empty())
	{
		cell r = q.top();
		q.pop();
		r.on(u1, u2);
		i1 = I1[u1];
		i2 = I2[u2];
		i = i1 + c * i2;
		U[n++] = i;
		V[i] = true;
		p += P[i];
		if(p >= t) break;
		if(u2 < w1 && (!u1 || v(u1-1, u2+1))) q.push(cell(u1, u2+1));
		if(u1 < w1 && (!u2 || v(u1+1, u2-1))) q.push(cell(u1+1, u2));
	}
}

//-----------------------------------------------------------------------------

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, mxArray const *prhs[])
{
	if (nrhs != 6) mexErrMsgTxt("Needs 6 input arguments");
	if (nlhs != 1) mexErrMsgTxt("Needs 1 output argument");

	mxArray      *&aC = plhs[0];  // n x 2
	mxArray const *aI = prhs[0],  // w x 2
	              *aS = prhs[1],  // w x 2
	              *aP = prhs[2],  // c x c
	              *aU = prhs[3],  // w x w
	              *aV = prhs[4],  // c x c
	              *aT = prhs[5];  // scalar

	int w = mxGetM(aI),
	    c = mxGetM(aP);

	if(mxGetM(aS) != w)
		mexErrMsgTxt("Dimensions of arguments 1,2 not consistent.");

	if(mxGetN(aP) != c)
		mexErrMsgTxt("Dimensions of argument 3 not consistent.");

	if(mxGetM(aU) != w || mxGetN(aU) != w)
		mexErrMsgTxt("Dimensions of argument 4 invalid.");

	if(mxGetM(aV) != c || mxGetN(aV) != c)
		mexErrMsgTxt("Dimensions of argument 5 invalid.");

	if(mxGetClassID(aI) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 1 should be of type uint32.");

	if(mxGetClassID(aS) != mxSINGLE_CLASS)
		mexErrMsgTxt("Argument 2 should be of type single.");

	if(mxGetClassID(aP) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 3 should be of type uint32.");

	if(mxGetClassID(aU) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 4 should be of type uint32.");

	if(mxGetClassID(aV) != mxUINT8_CLASS)
		mexErrMsgTxt("Argument 5 should be of type uint8.");

	if(mxGetClassID(aT) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 6 should be of type uint32.");

//-----------------------------------------------------------------------------

	unsigned      *I = (unsigned*)      mxGetPr(aI);      // cell sub-indices
	float         *S = (float*)         mxGetPr(aS);      // sub-distances
	unsigned      *P = (unsigned*)      mxGetPr(aP);      // cell populations
	unsigned      *U = (unsigned*)      mxGetPr(aU);      // cell indices buffer
	unsigned char *V = (unsigned char*) mxGetPr(aV);      // visited cells
	unsigned       t = (unsigned)       mxGetScalar(aT);  // target population

	int n;
	search_seq(I, S, P, U, V, t, w, c, n);

	aC = mxCreateNumericMatrix(n, 2, mxUINT32_CLASS, mxREAL);
	unsigned *C = (unsigned*) mxGetPr(aC);    // cell indices

	for(unsigned j = 0, *C1 = C, *C2 = C + n; j < n; ++j)
	{
		C1[j] = U[j] % c;
		C2[j] = U[j] / c;
		V[U[j]] = false;
	}
}
