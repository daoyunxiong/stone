#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "q2eul.h"
#include "gnss_sins_ekf_loose_integration_rtwutil.h"

void q2eul(const double q[4], double eul[3])
{
  double aSinInput;
  aSinInput = -2.0 * (q[1] * q[3] - q[0] * q[2]);
  if (aSinInput > 1.0) {
    aSinInput = 1.0;
  }

  eul[0] = rt_atan2d_snf(2.0 * (q[2] * q[3] + q[0] * q[1]), ((q[0] * q[0] - q[1]
    * q[1]) - q[2] * q[2]) + q[3] * q[3]);
  eul[1] = asin(aSinInput);
  eul[2] = rt_atan2d_snf(2.0 * (q[1] * q[2] + q[0] * q[3]), ((q[0] * q[0] + q[1]
    * q[1]) - q[2] * q[2]) - q[3] * q[3]);
}
