/** PerceptIn license **/

#ifndef PERCEPTIN_GEN1_INTERFACE_H_
#define PERCEPTIN_GEN1_INTERFACE_H_

#include <mutex>
#include <queue>
#include <thread>

#include <opencv2/core/core.hpp>
//#include <libuvc/libuvc.h>

namespace PI {

/**
 * Interface to stream stereo data and IMU data from the very first generation
 * of PerceptIn's device. This interface only allows the device to be turned on
 * and then off once.
 */
class PerceptInGen1Interface {
 public:

  /**
   * @brief Various modes for stereo images.
   */
  enum StereoFrameRate {
    /// Stream sterea images as 60 Hz.
    STEREO_FRAME_RATE_60HZ,
    /// Stream sterea images as 30 Hz.
    STEREO_FRAME_RATE_30HZ
  };

  /**
   * @brief Constructor.
   * @param stereo_frame_rate Flag of the frame rate of the stereo camera.
   * @param stream_imu Flag to specify whehter to stream IMU or not.
   */
  PerceptInGen1Interface(
      const StereoFrameRate stereo_frame_rate = STEREO_FRAME_RATE_60HZ,
      const bool stream_imu = true);
  virtual ~PerceptInGen1Interface();

  /**
   * @brief Wraps timestamp and stereo image into a data structure.
   */
  struct StereoData {
    /// Timestamp when the stereo image was captured.
    unsigned long timestamp;
    /// Stereo image, left and right
    cv::Mat_<uchar> image_left;
    cv::Mat_<uchar> image_right;

    StereoData();
    StereoData(const unsigned long _timestamp, const cv::Mat_<uchar> & _image_left, const cv::Mat_<uchar> & _image_right);
  };

  /**
   * @brief Wraps timestamps and IMU readings.
   */
   struct ImuData {
     unsigned long timestamp;
     cv::Vec3d accel;
     cv::Vec3d gyro;

     ImuData();
     ImuData(const unsigned long timestamp,
             const double accel_x, const double accel_y, const double accel_z,
             const double gyro_x, const double gyro_y, const double gyro_z);
   };

  /**
   * @brief Initialize UVC, find device, and then start streaming stereo images
   *        and IMU (if requested).
   * @return True if successful. False otherewise.
   */
  bool StartStreaming();

  /**
   * @brief Stop streaming data, and exit UVC device.
   * @return True if successful. False otherwise.
   */
  bool StopStreaming();

  /**
   * @brief Get exposure of the stereo camera.
   * @param[out] exposure Value of the exposure read from the driver. 
   * @return True if successful. False otherwise.
   */
  bool GetExposure(uint32_t *exposure) const;

  /**
   * @brief Set exposure of the stereo camera.
   * @param exposure Value to set to. 
   * @return True if successful. False otherwise.
   */
  bool SetExposure(const uint32_t exposure);

  /**
   * @brief Get the oldest stereo data from the streaming queue. This API will
   *        just pop the oldest data in the queue.
   * @param[out] stereo_data The oldest stereo data.
   * @return True if the oldest stereo data is available. If false, do NOT use
   *         the output #stereo_data.
   */
  bool GetStereoDataFromQueue(StereoData *stereo_data);
  /**
   * @brief Get the newest stereo data from the streaming queue. This API will
   *        drop all the previous data in the queue.
   * @param[out] stereo_data The newest stereo data.
   * @return True if the newest stereo data is available. If false, do NOT use
   *         the output #stereo_data.
   */
  bool GetStereoData(StereoData *stereo_data);

  /**
   * @brief Get the newest IMU data from the streaming queue. This API will
   *        drop all the previous data in the queue. -- no, GetImuData won't
   * @param imu_data The newest IMU data.
   * @return True if the newest IMU data is available. If false, do NOT use the
   *         output #imu_data.
   */
  bool GetImuData(ImuData *imu_data);

  /**
   * @brief Convert the UVC frame into a stereo image in cv::Mat, and add the
   *        image to the stereo queue.
   * @param frame The UVC frame to convert.
   */
 // void StreamStereo(uvc_frame_t *frame);

 private:

  /**
   * @brief Pull an IMU reading from UVC, convert into IMU data, and add to the
   *        IMU queue.
   */
  void StreamImu();

  /// Frame rate of the stereo camera.
  const int frame_rate_stereo_hz_;
  /// A flag to specify whether to stream IMU or not.
  const bool stream_imu_;

  /// UVC
 // uvc_context_t *ctx_;
 // uvc_device_t *dev_;
 // uvc_device_handle_t *devh_;

  /// Stereo data to be deployed.
  std::queue<StereoData> queue_stereo_;
  /// Mutex to protect #queue_stereo_.
  std::mutex mutex_queue_stereo_;

  /// IMU data to be deployed.
  std::queue<ImuData> queue_imu_;
  /// Mutex to protect #queue_imu_.
  std::mutex mutex_queue_imu_;
  /// Thread for streaming IMU into #queue_imu_.
  std::thread thread_stream_imu_;
  /// set true to break out loop of stream IMU thread
  bool stopStreamImuThread;

  /// A flag specifying the state of the interface. If true, data are streaming
  /// from the device. False otherwise.
  bool streaming_;
  std::mutex mutex_streaming_;
};

}  // namespace PI

#endif  // PERCEPTIN_GEN1_INTERFACE_H_
