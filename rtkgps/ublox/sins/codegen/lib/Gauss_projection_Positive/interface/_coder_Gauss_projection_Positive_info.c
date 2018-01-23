#include "_coder_Gauss_projection_Positive_info.h"

const mxArray *emlrtMexFcnResolvedFunctionsInfo(void)
{
  const mxArray *nameCaptureInfo;
  const char * data[11] = {
    "789ced5a4b6fd3401076a11490780971008404a70a5411178a00c1a5054a1bdaa240036a55a166936ce882ed35b65382b8449c3872e4c82fe106278ef043b8c0"
    "0989ddd826eee2768d1f8d1dc692654decd9f9e6b533634719292f29ec38c2ce938f15658c5d0fb0738fe21efb3c7a849d67bcabfbfba872d8a3dfb0b3410d07",
    "771cf7a68174acf84793eac44086537d6562c5c236d53671b377a745345c253a5ea401629e3042bf1bb8f587e0b7ac0dfbcfca8a1624dc83ebf156e9eb311aa2"
    "c783801ec73c7a6df6c9ec0df525b59edb266a6055478e86ea172f4f5eba3679656ad2a3d9c55c372dfa0c371c420d750eb56d3bf0c37a85dac4219bb8a407f0",
    "5c97e03920e0e1b46e35c92669e228fc6302ff58cfeaedba865df95d09ff3d819fd36be5c5959e492a167d6a21fd1c7782ad2ecd5417676ea90f995dae22d5a1"
    "54abd38e8a754dd548dd3712356dd5c75f32c3ed301682632480e3a0f73b3b0e4d7ff83693803fa9fc5f174ea523ff7c807f24845f095ce33cdf55f2e7e7301c",
    "fb051c9cf69689c49f34decb027f39053bf866d82eefa3c74bf7f5f7ca40e33d35fe41c7fb6ef9b922c17156c0c1e9066d62ab4458d1b40ca495887dab4d34a7"
    "6cdc6febd8228d54f24056075704fe9538f6e1e7444f1b75c2574715d529e909e2e9e48f2f5f211f223cdf558a910fe7041c9c16f2c16e200d77ccdb54379143",
    "5844a7910fef24fcab02ff6a1cfb84e6c35feaa45027949f69f52583e6ff5ff2625e82e3b88083d3425eb055c3ed9e459fb420f02fc4b14b683e30353ccb24e8"
    "936ce893a23d2feb03767b1e9e94e0d9bb05cf5ea5453aa9c47b47c23f27f0733ac93e80b556db50197aa67d8238397c7e1ce23ccaf383f62fcc013007401ec8",
    "f3e094209fd321fd3fb24a7ce7efebb553ddf08fe0ba3e9f2cfe9705bee538f6088dff09570fcf363e9e98fdce0fe8f78b11ff3589fc0b827c4e0bf18f4c537b"
    "b5dc0b9ebb6da3d74b958d8ac63ab1809ed312394705399c6e79abad6f20a3a9b9ef5bdf4bd6a90bebd4e3d82b343fb657d36f1813c4dbc7cf53902f519ecfbb",
    "ffb3aa2759f7555057862b4f64fecefb1cdda07681e768861ee6e821ee9ffafe4d6fbfe711dfd7abb8f3836b1b1f0fecf3c31dff35897c981f607ec853bee4dd"
    "ff59d593e2cf0f505782fc303fec3c3f38c828f0fcc0d0c3fc30c4fd53dfbfe9edf73ce2fb7a15777e706de3e3817d7eb8e3bf26910ff303cc0f79ca97bcfb3f",
    "ab7a52fcf901ea4a901fe687ad7846b7e01955ec1796b31bf3c3bcc0cfe9e4f595a34ff4ff25981f223e3f68ffcaf6fb33827c4efff53fbe3b5447c498b52c6a"
    "f97a4d4bd68dda2fc9f68147c23a8fe2d8679bfff305d4eaed0509e269fc26e44321f2615e22ff84209fd3423ee0601efc6bdd686c208bf375257c4b028ea538",
    "76088d7b9c42bc773fddc410ef458877d9fe7f5a90cfe9f07e9fafd9d72babf7475581af1ac71e3bf6f9ae6d7c3cd0e70f77fcd724f2e1fd11bc3fca53bee4dd"
    "ff59d593acdf1f415d19ae3c91f93b6fef8fc4efcf3629f2f767861ebe3fe7a27fcadabfe97d2fe011dfd7abb8df9f5ddbf878609f1feef8af49e4c3fc00f343",
    "9ef225effecfaa9e14fffb33d495207f9679f21b62228b9d", "" };

  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(data, 22056U, &nameCaptureInfo);
  return nameCaptureInfo;
}

mxArray *emlrtMexFcnProperties(void)
{
  mxArray *xResult;
  mxArray *xEntryPoints;
  const char * fldNames[4] = { "Name", "NumberOfInputs", "NumberOfOutputs",
    "ConstantInputs" };

  mxArray *xInputs;
  const char * b_fldNames[4] = { "Version", "ResolvedFunctions", "EntryPoints",
    "CoverageInfo" };

  xEntryPoints = emlrtCreateStructMatrix(1, 1, 4, fldNames);
  xInputs = emlrtCreateLogicalMatrix(1, 3);
  emlrtSetField(xEntryPoints, 0, "Name", mxCreateString(
    "Gauss_projection_Positive"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs", mxCreateDoubleScalar(3.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs", mxCreateDoubleScalar(2.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  xResult = emlrtCreateStructMatrix(1, 1, 4, b_fldNames);
  emlrtSetField(xResult, 0, "Version", mxCreateString("9.0.0.341360 (R2016a)"));
  emlrtSetField(xResult, 0, "ResolvedFunctions", (mxArray *)
                emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}
