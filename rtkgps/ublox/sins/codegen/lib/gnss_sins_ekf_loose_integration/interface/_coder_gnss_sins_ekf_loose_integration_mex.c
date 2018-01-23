#include "_coder_gnss_sins_ekf_loose_integration_api.h"
#include "_coder_gnss_sins_ekf_loose_integration_mex.h"

static void c_gnss_sins_ekf_loose_integrati(int32_T nlhs, mxArray *plhs[7],
  int32_T nrhs, const mxArray *prhs[7]);
static void c_gnss_sins_ekf_loose_integrati(int32_T nlhs, mxArray *plhs[7],
  int32_T nrhs, const mxArray *prhs[7])
{
  int32_T n;
  const mxArray *inputs[7];
  const mxArray *outputs[7];
  int32_T b_nlhs;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  if (nrhs != 7) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 7, 4,
                        31, "gnss_sins_ekf_loose_integration");
  }

  if (nlhs > 7) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 31,
                        "gnss_sins_ekf_loose_integration");
  }

  for (n = 0; n < nrhs; n++) {
    inputs[n] = prhs[n];
  }

  gnss_sins_ekf_loose_integration_api(inputs, outputs);
  if (nlhs < 1) {
    b_nlhs = 1;
  } else {
    b_nlhs = nlhs;
  }

  emlrtReturnArrays(b_nlhs, plhs, outputs);
  gnss_sins_ekf_loose_integration_terminate();
}

void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs, const mxArray
                 *prhs[])
{
  mexAtExit(gnss_sins_ekf_loose_integration_atexit);
  gnss_sins_ekf_loose_integration_initialize();
  c_gnss_sins_ekf_loose_integrati(nlhs, plhs, nrhs, prhs);
}

emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  return emlrtRootTLSGlobal;
}
