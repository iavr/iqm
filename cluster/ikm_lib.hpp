#include <queue>
#include <algorithm>
#include <cmath>
#include <limits>
#include "mex.h"

#define INLINE __attribute__((always_inline))

using std::vector;
using std::priority_queue;

//-----------------------------------------------------------------------------
// std::iota of C++11

template<typename I, typename T>
void iota(I b, I e, T x) { while(b != e) *b++ = x++; }

//-----------------------------------------------------------------------------
// handy infinity

float inf() { return std::numeric_limits<float>::infinity(); }
