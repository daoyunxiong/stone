#include "tmwtypes.h"
#include "_coder_gnss_sins_ekf_loose_integration_api.h"
#include "_coder_gnss_sins_ekf_loose_integration_mex.h"

emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true, false, 131434U, NULL,
  "gnss_sins_ekf_loose_integration", NULL, false, { 2045744189U, 2170104910U,
    2743257031U, 4284093946U }, NULL };

static real_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static const mxArray *b_emlrt_marshallOut(const real_T u[225]);
static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *imus,
  const char_T *identifier))[7];
static const mxArray *c_emlrt_marshallOut(const real_T u[115]);
static real_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[7];
static real_T (*e_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *gps_pos_llas, const char_T *identifier))[3];
static real_T emlrt_marshallIn(const emlrtStack *sp, const mxArray *tau, const
  char_T *identifier);
static const mxArray *emlrt_marshallOut(const real_T u[3]);
static real_T (*f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[3];
static real_T g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId);
static real_T (*h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[7];
static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[3];
static real_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = g_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static const mxArray *b_emlrt_marshallOut(const real_T u[225])
{
  const mxArray *y;
  const mxArray *m1;
  static const int32_T iv2[2] = { 0, 0 };

  static const int32_T iv3[2] = { 15, 15 };

  y = NULL;
  m1 = emlrtCreateNumericArray(2, iv2, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m1, (void *)u);
  emlrtSetDimensions((mxArray *)m1, iv3, 2);
  emlrtAssign(&y, m1);
  return y;
}

static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *imus,
  const char_T *identifier))[7]
{
  real_T (*y)[7];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(imus), &thisId);
  emlrtDestroyArray(&imus);
  return y;
}
  static const mxArray *c_emlrt_marshallOut(const real_T u[115])
{
  const mxArray *y;
  const mxArray *m2;
  static const int32_T iv4[1] = { 0 };

  static const int32_T iv5[1] = { 115 };

  y = NULL;
  m2 = emlrtCreateNumericArray(1, iv4, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m2, (void *)u);
  emlrtSetDimensions((mxArray *)m2, iv5, 1);
  emlrtAssign(&y, m2);
  return y;
}

static real_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[7]
{
  real_T (*y)[7];
  y = h_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}
  static real_T (*e_emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *gps_pos_llas, const char_T *identifier))[3]
{
  real_T (*y)[3];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = f_emlrt_marshallIn(sp, emlrtAlias(gps_pos_llas), &thisId);
  emlrtDestroyArray(&gps_pos_llas);
  return y;
}

static real_T emlrt_marshallIn(const emlrtStack *sp, const mxArray *tau, const
  char_T *identifier)
{
  real_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(tau), &thisId);
  emlrtDestroyArray(&tau);
  return y;
}

static const mxArray *emlrt_marshallOut(const real_T u[3])
{
  const mxArray *y;
  const mxArray *m0;
  static const int32_T iv0[2] = { 0, 0 };

  static const int32_T iv1[2] = { 1, 3 };

  y = NULL;
  m0 = emlrtCreateNumericArray(2, iv0, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m0, (void *)u);
  emlrtSetDimensions((mxArray *)m0, iv1, 2);
  emlrtAssign(&y, m0);
  return y;
}

static real_T (*f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[3]
{
  real_T (*y)[3];
  y = i_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}
  static real_T g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId)
{
  real_T ret;
  static const int32_T dims = 0;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 0U, &dims);
  ret = *(real_T *)mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

static real_T (*h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[7]
{
  real_T (*ret)[7];
  static const int32_T dims[2] = { 1, 7 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 2U, dims);
  ret = (real_T (*)[7])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
  static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[3]
{
  real_T (*ret)[3];
  static const int32_T dims[2] = { 1, 3 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 2U, dims);
  ret = (real_T (*)[3])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

void gnss_sins_ekf_loose_integration_api(const mxArray *prhs[7], const mxArray
  *plhs[7])
{
  real_T (*pos_ins_s_t)[3];
  real_T (*vel_ins_s_t)[3];
  real_T (*eul_ins_s_t)[3];
  real_T (*accelBias_s_t)[3];
  real_T (*gyroBias_s_t)[3];
  real_T (*P_t)[225];
  real_T (*FeedBacks_t)[115];
  real_T tau;
  real_T (*imus)[7];
  real_T (*gps_pos_llas)[3];
  real_T (*gps_vel_neds)[3];
  real_T (*initial_accel_bias)[3];
  real_T (*initial_gyro_bias)[3];
  real_T flag;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  pos_ins_s_t = (real_T (*)[3])mxMalloc(sizeof(real_T [3]));
  vel_ins_s_t = (real_T (*)[3])mxMalloc(sizeof(real_T [3]));
  eul_ins_s_t = (real_T (*)[3])mxMalloc(sizeof(real_T [3]));
  accelBias_s_t = (real_T (*)[3])mxMalloc(sizeof(real_T [3]));
  gyroBias_s_t = (real_T (*)[3])mxMalloc(sizeof(real_T [3]));
  P_t = (real_T (*)[225])mxMalloc(sizeof(real_T [225]));
  FeedBacks_t = (real_T (*)[115])mxMalloc(sizeof(real_T [115]));
  prhs[1] = emlrtProtectR2012b(prhs[1], 1, false, -1);
  prhs[2] = emlrtProtectR2012b(prhs[2], 2, false, -1);
  prhs[3] = emlrtProtectR2012b(prhs[3], 3, false, -1);
  prhs[4] = emlrtProtectR2012b(prhs[4], 4, false, -1);
  prhs[5] = emlrtProtectR2012b(prhs[5], 5, false, -1);
  tau = emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "tau");
  imus = c_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "imus");
  gps_pos_llas = e_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "gps_pos_llas");
  gps_vel_neds = e_emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "gps_vel_neds");
  initial_accel_bias = e_emlrt_marshallIn(&st, emlrtAlias(prhs[4]),
    "initial_accel_bias");
  initial_gyro_bias = e_emlrt_marshallIn(&st, emlrtAlias(prhs[5]),
    "initial_gyro_bias");
  flag = emlrt_marshallIn(&st, emlrtAliasP(prhs[6]), "flag");
  gnss_sins_ekf_loose_integration(tau, *imus, *gps_pos_llas, *gps_vel_neds,
    *initial_accel_bias, *initial_gyro_bias, flag, *pos_ins_s_t, *vel_ins_s_t,
    *eul_ins_s_t, *accelBias_s_t, *gyroBias_s_t, *P_t, *FeedBacks_t);
  plhs[0] = emlrt_marshallOut(*pos_ins_s_t);
  plhs[1] = emlrt_marshallOut(*vel_ins_s_t);
  plhs[2] = emlrt_marshallOut(*eul_ins_s_t);
  plhs[3] = emlrt_marshallOut(*accelBias_s_t);
  plhs[4] = emlrt_marshallOut(*gyroBias_s_t);
  plhs[5] = b_emlrt_marshallOut(*P_t);
  plhs[6] = c_emlrt_marshallOut(*FeedBacks_t);
}

void gnss_sins_ekf_loose_integration_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  gnss_sins_ekf_loose_integration_xil_terminate();
}

void gnss_sins_ekf_loose_integration_initialize(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

void gnss_sins_ekf_loose_integration_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}
