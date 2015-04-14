//-----------------------------------------------------------------------------
// update step. all cells are scanned and their contribution is collected to
// centroid population, weighted variance, and weighted mean (vector),
// for the centroid to which they are assigned. populations are used as weights.

void update(
	int K, int D, int c, int ce, unsigned *p, float *W, float *s,
	unsigned const *P, unsigned const *Mi, float const *M,
	unsigned const *A, float const *Z
)
{
	std::fill(p, p + K, 0u);              // initialize population, variance and
	std::fill(s, s + K, .0f);             // mean to zero for each centroid
	std::fill(W, W + D * K, .0f);

	// for all cells j
	for(int j = 0, g = c * c; j < g; ++j)
	{
		unsigned k = A[j];                 // assigned centroid
		float q = P[j];                    // cell population
		if(!q || k >= K) continue;         // skip unassigned or empty cells
		p[k] += P[j];                      // contribute to centroid population
		s[k] += q * Z[j];                  // contribute to centroid (weighted) variance
		float *Wk = W + D * k;             // offset of k-th centroid
		float const *Mj = M + D * Mi[j];   // offset of j-th cell mean
		for(int d = 0; d < D; ++d)         // contribute to centroid (weighted) mean
			Wk[d] += q * Mj[d];
	}

	// for all centroids k
	for(int k = 0; k < K; ++k, W += D)
	{
		float q = p[k];                    // centroid population
		if(!q) continue;                   // skip empty centroids
		s[k] /= q;                         // normalize variance by total population
		for(int d = 0; d < D; ++d)         // normalize mean by total population
			W[d] /= q;
	}
}
