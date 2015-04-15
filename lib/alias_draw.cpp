#include "mex.h"
#include "mex.hpp"

//-----------------------------------------------------------------------------
// Alias method - O(N) sampling.
// See Devroye 1986, Non-Uniform Random Variate Generation.
// [http://luc.devroye.org/rnbookindex.html]
// Attached "alias.pdf" containing p. 107-109 of the book.

// F: floating point type
// I: integral type
// K: number of outcomes
// N: number of samples
// X: output samples
// Q: input alias probabilities
// J: input alias table
// R: input uniform sample 1
// V: input uniform sample 2

template<typename F, typename I>
void alias_draw(
	int K, int N, I *X,
	F const *Q, I const *J, F const *R, F const *V
)
{
	// Distribution expressed as uniform mixture of two-point distributions.
	// 1st sample R, uniform in [0,1], picks the mixture component x.
	// 2nd sample V, also uniform in [0,1], picks one of the two outcomes
	// x, J[x] in the two-point distribution, with probabilities Q[x], 1-Q[x].
	for(int n = 0; n < N; ++n)
	{
		I x = K * R[n];                    // Uniform on 0..K-1
		X[n] = V[n] <= Q[x] ? x : J[x];    // Two-point with probability Q[x]
	}
}

//-----------------------------------------------------------------------------
// mex entry point templated on types
// F: floating point type
// I: integral type

template<typename F, typename I>
void mex(int nlhs, mxArray *plhs[],
         int nrhs, mxArray const *prhs[])
{
	if (nrhs != 4)  mexErrMsgTxt("Needs 4 input arguments");
	if (nlhs != 1)  mexErrMsgTxt("Needs 1 output arguments");

	mxArray      *&aX = plhs[0];  // 1 x N   output samples
	mxArray const *aQ = prhs[0],  // 1 x K   input alias probabilities
	              *aJ = prhs[1],  // 1 x K   input alias table
	              *aR = prhs[2],  // 1 x N   input uniform sample 1
	              *aV = prhs[3];  // 1 x N   input uniform sample 2

	int K = mxGetN(aQ),     // number of outcomes
	    N = mxGetN(aR);     // number of samples

	if(mxGetM(aQ) != 1)
		mexErrMsgTxt("Dimensions of argument 1 invalid.");

	if(mxGetM(aJ) != 1 || mxGetN(aJ) != K)
		mexErrMsgTxt("Dimensions of arguments 1,2 inconsistent.");

	if(mxGetM(aR) != 1)
		mexErrMsgTxt("Dimensions of argument 3 invalid.");

	if(mxGetM(aV) != 1 || mxGetN(aV) != N)
		mexErrMsgTxt("Dimensions of arguments 3,4 inconsistent.");

	if(mxGetClassID(aQ) != float_type<F>())
		mexErrMsgTxt("Argument 1 has incorrect floating point class.");

	if(mxGetClassID(aJ) != int_type<I>())
		mexErrMsgTxt("Argument 2 has incorrect integer class.");

	if(mxGetClassID(aR) != float_type<F>())
		mexErrMsgTxt("Argument 3 has incorrect floating point class.");

	if(mxGetClassID(aV) != float_type<F>())
		mexErrMsgTxt("Argument 4 has incorrect floating point class.");

	aX = mxCreateNumericMatrix(1, N, int_type<I>(), mxREAL);

	I *X = (I*) mxGetPr(aX);    // output samples
	F *Q = (F*) mxGetPr(aQ);    // input alias probabilities
	I *J = (I*) mxGetPr(aJ);    // input alias table
	F *R = (F*) mxGetPr(aR);    // input uniform sample 1
	F *V = (F*) mxGetPr(aV);    // input uniform sample 2

	alias_draw(K, N, X, Q, J, R, V);
}

//-----------------------------------------------------------------------------
// mex entry point -- choose types

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, mxArray const *prhs[])
{
	mex<double,unsigned>(nlhs, plhs, nrhs, prhs);
}
