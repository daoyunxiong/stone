//=====================================================================================================
// MahonyAHRS.h
//=====================================================================================================
//
// Madgwick's implementation of Mayhony's AHRS algorithm.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author			Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================
#ifndef MahonyAHRS_h
#define MahonyAHRS_h

//----------------------------------------------------------------------------------------------------
// Variable declaration

class MahonyAHRS{
  private:
    float twoKp;			// 2 * proportional gain (Kp)
    float twoKi;			// 2 * integral gain (Ki)
    float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame
    float integralFBx;
    float integralFBy; 
    float integralFBz; 
    float yaw;
    float pitch;
    float roll;
    float anglesComputed;
  
  //---------------------------------------------------------------------------------------------------
  // Function declarations
  public:
    MahonyAHRS();
    float invSqrt(float x);
    void update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
    void updateIMU(float gx, float gy, float gz, float ax, float ay, float az);
    void computeAngles();
    float getYaw()
    {
	computeAngles();
        return yaw * 57.29578f + 180.0f;
    }
    float getRoll()
    {
	computeAngles();
        return roll;
    }
    float getPitch()
    {
	computeAngles();
        return pitch;
    }
};
#endif
//=====================================================================================================
// End of file
//=====================================================================================================
