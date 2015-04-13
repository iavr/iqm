#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
	if (nrhs != 2) mexErrMsgTxt("Needs 2 input arguments");
	if (nlhs != 1) mexErrMsgTxt("Needs 1 output argument");

	mxArray *&     aD = plhs[0];  // n x 1
	const mxArray *aL = prhs[0],  // m x n
	              *aE = prhs[1];  // k x m

	int k = mxGetM(aL),
	    m = mxGetM(aE),
	    n = mxGetN(aE);

	if(mxGetN(aL) != m)
		mexErrMsgTxt("Dimensions of arguments 1,2 not consistent.");

	if(mxGetClassID(aL) != mxSINGLE_CLASS)
		mexErrMsgTxt("Argument 1 should be of type single.");

	if(mxGetClassID(aE) != mxUINT8_CLASS)
		mexErrMsgTxt("Argument 2 should be of type uint8.");

	aD = mxCreateNumericMatrix(n, 1, mxSINGLE_CLASS, mxREAL);

	float         *D = (float*)         mxGetPr(aD);  // distances
	float         *L = (float*)         mxGetPr(aL);  // lookup table
	unsigned char *E = (unsigned char*) mxGetPr(aE);  // codes

	for(int i = 0, l = m * k; i != n; ++i)
	{
		float d = 0;
		for(int j = 0; j < l; j += k)
			d += L[*E++ + j];
		D[i] = d;
	}
}
