#include "ikm_lib.hpp"
#include "ikm_search.hpp"
#include "ikm_update.hpp"
#include "ikm_search_cen.hpp"
#include "ikm_purge.hpp"

//-----------------------------------------------------------------------------

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, mxArray const *prhs[])
{
	if (nrhs != 13) mexErrMsgTxt("Needs 13 input arguments");
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
	              *aQ = prhs[11], // c x c     quantized centroid per cell
	              *aT = prhs[12]; // scalar    target population

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

	if(mxGetM(aQ) != c || mxGetN(aQ) != c)
		mexErrMsgTxt("Dimensions of argument 12 invalid.");

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

	if(mxGetClassID(aQ) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 12 should be of type uint32.");

	if(mxGetClassID(aT) != mxUINT32_CLASS)
		mexErrMsgTxt("Argument 13 should be of type uint32.");

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
	unsigned      *Q = (unsigned*)      mxGetPr(aQ);      // quantized centroid/cell
	unsigned       t = (unsigned)       mxGetScalar(aT);  // target population

	visit v(U, V, c);
	term T(t, P);
	search(K, w, p, I, S, v, T, cell_nn(A, Z));
	update(K, D, c, p, W, s, P, M, A, Z);

	nhood N(20, K);  // TODO: centroid neighbors parameter
	quantize(K, w, c, p, I, Q);
	search(K, w, p, I, S, v, T, cen_nn(N, Q));
	unquantize(K, w, c, I, Q);
	purge(K, D, p, s, N);
}
