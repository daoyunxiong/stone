#ifndef _CODER_GNSS_SINS_EKF_LOOSE_INTEGRATION_MEX_H
#define _CODER_GNSS_SINS_EKF_LOOSE_INTEGRATION_MEX_H
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include "_coder_gnss_sins_ekf_loose_integration_api.h"

extern void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs, const
  mxArray *prhs[]);
extern emlrtCTX mexFunctionCreateRootTLS(void);

#endif
