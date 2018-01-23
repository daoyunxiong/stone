#ifndef _CODER_GAUSS_PROJECTION_POSITIVE_API_H
#define _CODER_GAUSS_PROJECTION_POSITIVE_API_H
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_Gauss_projection_Positive_api.h"

extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;
extern void Gauss_projection_Positive(real_T B, real_T L, real_T Coordinate,
  real_T *x, real_T *y);
extern void Gauss_projection_Positive_api(const mxArray * const prhs[3], const
  mxArray *plhs[2]);
extern void Gauss_projection_Positive_atexit(void);
extern void Gauss_projection_Positive_initialize(void);
extern void Gauss_projection_Positive_terminate(void);
extern void Gauss_projection_Positive_xil_terminate(void);

#endif
