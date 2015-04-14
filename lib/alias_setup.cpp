#include "mex.h"
#include "mex.hpp"

//-----------------------------------------------------------------------------
// Alias method - O(K) table setup.
// See Devroye 1986 - Non-Uniform Random Variate Generation.
// [http://luc.devroye.org/rnbookindex.html]
// Attached "alias.pdf" containing p. 107-109 of the book.

// K: number of outcomes
// Q: output alias probabilities
// J: output alias table
// P: input probability distribution (not necessarily normalized)

template<typename F, typename I>
void alias_setup(int K, F *Q, I *J, F const *P)
{
	F s = 0;
	for(int i = 0; i < K; ++i)
		s += P[i];                        // sum of probabilities, for normalization

	int *T = new int[K];                 // table holding two lists of indices
	int il = -1, ik = K;                 // pointers into heads of two lists

	for(int i = 0; i < K; ++i)
	{
		Q[i] = K * P[i] / s;              // probabilities normalized to K
		T[Q[i] < 1 ? ++il : --ik] = i;    // left/right lists of T hold elements
		                                  // that are < / >= 1 (i.e. "small/large")
	}

	while(il >= 0 && ik < K)             // while no list is empty
	{
		int l = T[il];                    // top element of left ("small") list
		int k = T[ik];                    // top element of right ("large") list
		J[l] = k;                         // "small" element aliases to "large"
		Q[k] += Q[l] - 1;                 // complement of "small" probability ..
		                                  // .. removed from "large"
		if(Q[k] < 1)                      // if "large" element becomes "small"
		{
			T[il] = k;                     // then "large" replaces "small" at top ..
			                               // .. of "small" list ..
			++ik;                          // .. and removed from "large" list
		}
		else --il;                        // otherwise "small" element removed ..
		                                  // from "small" list
	}

	delete[] T;
}

//-----------------------------------------------------------------------------

template<typename F, typename I>
void mex(int nlhs, mxArray *plhs[],
         int nrhs, mxArray const *prhs[])
{
	if (nrhs != 1)  mexErrMsgTxt("Needs 1 input arguments");
	if (nlhs != 2)  mexErrMsgTxt("Needs 2 output arguments");

	mxArray      *&aQ = plhs[0],  // 1 x K
	             *&aJ = plhs[1];  // 1 x K
	mxArray const *aP = prhs[0];  // m x n (K = m*n)

	int m = mxGetM(aP),
	    n = mxGetN(aP),
	    K = m * n;         // number of outcomes

	if(mxGetClassID(aP) != float_type<F>())
		mexErrMsgTxt("Argument 1 has incorrect floating point class.");

	aQ = mxCreateNumericMatrix(1, K, float_type<F>(), mxREAL);
	aJ = mxCreateNumericMatrix(1, K, int_type<I>(),   mxREAL);

	F *Q = (F*) mxGetPr(aQ);    // output alias probabilities
	I *J = (I*) mxGetPr(aJ);    // output alias table
	F *P = (F*) mxGetPr(aP);    // input probability distribution

	alias_setup(K, Q, J, P);
}

//-----------------------------------------------------------------------------

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, mxArray const *prhs[])
{
	mex<double,unsigned>(nlhs, plhs, nrhs, prhs);
}
