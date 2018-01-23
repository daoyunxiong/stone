/** PerceptIn license **/

#include <map>

#include <glog/logging.h>

#include <perceptin_gen1_interface.h>

#define US_TO_SEC 1000000

namespace PI {

// Map from StereoFrameRate to actual frame rate in Hz.
const std::map<PerceptInGen1Interface::StereoFrameRate, int>
    kStereoFrameRateToHz = {
        {PerceptInGen1Interface::StereoFrameRate::STEREO_FRAME_RATE_60HZ, 60},
        {PerceptInGen1Interface::StereoFrameRate::STEREO_FRAME_RATE_30HZ, 30}};

PerceptInGen1Interface::StereoData::StereoData() {
}

PerceptInGen1Interface::StereoData::StereoData(
    unsigned long _timestamp,
    const cv::Mat_<uchar> & _image_left,
    const cv::Mat_<uchar> & _image_right) :
    timestamp(_timestamp),
    image_left(_image_left),
    image_right(_image_right) {
}

PerceptInGen1Interface::ImuData::ImuData() {
}

PerceptInGen1Interface::ImuData::ImuData(
    const unsigned long timestamp,
    const double accel_x, const double accel_y, const double accel_z,
    const double gyro_x, const double gyro_y, const double gyro_z) :
    timestamp(timestamp),
    accel(accel_x, accel_y, accel_z),
    gyro(gyro_x, gyro_y, gyro_z) {
}

PerceptInGen1Interface::PerceptInGen1Interface(
    const StereoFrameRate stereo_frame_rate /* = STEREO_FRAME_RATE_60HZ*/,
    const bool stream_imu /* = true */) :
    frame_rate_stereo_hz_(kStereoFrameRateToHz.at(stereo_frame_rate)),
    stream_imu_(stream_imu) {
  streaming_ = false;
}

PerceptInGen1Interface::~PerceptInGen1Interface() {
  if (streaming_) {
    StopStreaming();
  }
}

/**
 * @brief The callback function for UVC stereo image stream.
 * @details This callback function runs once per frame. This function calls
 *          PerceptInGen1Interface::StreamStereo(). If this function takes too
 *          long, we'll start losing frames.
 * @param frame Frame buffer from UVC
 * @param ptr Pointer to the PerceptInGen1Interface object.
 */
void CallbackStereo(uvc_frame_t *frame, void *ptr) {
  static_cast<PerceptInGen1Interface*>(ptr)->StreamStereo(frame);
}

bool PerceptInGen1Interface::StartStreaming() {
  // Don't start streaming again!
  if (streaming_) {
    return false;
  }

  uvc_stream_ctrl_t ctrl;
  uvc_error_t res;

  res = uvc_init(&ctx_, NULL);
  if (res < 0) {
    printf("(UVC) Init failed. :(\n");
    return false;
  }
  printf("(UVC) Initialized. :)\n");

  res = uvc_find_device(ctx_, &dev_, 0x2a0b, 0x00f5, NULL);
  if (res < 0) {
    printf("(UVC) Failed to find device. :(\n");
    return false;
  }
  printf("(UVC) Device found.  :)\n");

  res = uvc_open(dev_, &devh_);
  if (res < 0) {
    printf("(UVC) Failed to open device. (Do you have root access?) :(\n");
    return false;
  }
  printf("(UVC) Device opened.  :)\n");

  // Start the video stream.
  res = uvc_get_stream_ctrl_format_size(
      devh_, &ctrl, UVC_FRAME_FORMAT_YUYV, 640, 480, frame_rate_stereo_hz_);
  if (res < 0) {
    printf("(UVC) Failed to find matching mode.   :(\n");
    return false;
  }

  // Disable auto exposure.
  res = uvc_disable_ae_mode(devh_);
  if (res < 0) {
    printf("(UVC) Failed to turn off auto-exposure. :(\n");
  }
//  res = uvc_set_exposure(devh_, 250);
//  if (res < 0) {
//    printf("(UVC) Failed to set exposure to 250 :(\n");
//  }
  res = uvc_start_streaming(devh_, &ctrl, CallbackStereo, this, 0);
  if (res < 0) {
    printf("(UVC) Failed to start streaming.  :(\n");
    return false;
  }
  printf("(UVC) Start streaming stereo images.  :)\n");

  // Start the IMU stream.
  if (stream_imu_) {
    stopStreamImuThread = false;
    thread_stream_imu_ = std::thread(&PerceptInGen1Interface::StreamImu, this);
    printf("(UVC) Start streaming IMU.  :)\n");
  }

  streaming_ = true;
  return true;
}

bool PerceptInGen1Interface::StopStreaming() {
  if (!streaming_) {
    return false;
  }

  // Stop streaming.
  uvc_stop_streaming(devh_);
  printf("(UVC) Stopped streaming. :)\n");

  stopStreamImuThread = true;
  // Wait for IMU streaming thread to join.
  if (thread_stream_imu_.joinable()) {
    thread_stream_imu_.join();
    printf("(UVC) Stopped streaming IMU.  :)\n");
  }

  uvc_close(devh_);
  uvc_unref_device(dev_);
  printf("(UVC) Device closed. :)\n");

  uvc_exit(ctx_);
  printf("(UVC) Exited.  :)\n");

  // Mark the state to not streaming.
  streaming_ = false;
  return true;
}

bool PerceptInGen1Interface::GetExposure(uint32_t *exposure) const {
  if (!streaming_) {
    return false;
  }

  uvc_error_t res = uvc_get_exposure(devh_, exposure);
  if (res < 0) {
    printf("(UVC) Failed to get exposure. :(\n");
    return false;
  }
  return true;
}

bool PerceptInGen1Interface::SetExposure(const uint32_t exposure) {
  if (!streaming_) {
    return false;
  }

  uvc_error_t res = uvc_set_exposure(devh_, exposure);
  if (res < 0) {
    printf("(UVC) Failed to set exposure to %d :(\n", exposure);
    return false;
  }
  return true;
}

bool PerceptInGen1Interface::GetStereoDataFromQueue(StereoData *stereo_data) {
  CHECK_NOTNULL(stereo_data);

  mutex_streaming_.lock();
  bool streaming = streaming_;
  mutex_streaming_.unlock();
  if (!streaming) {
    return false;
  }

  // Check if the queue is empty.
  mutex_queue_stereo_.lock();
  if(queue_stereo_.empty()) {
    mutex_queue_stereo_.unlock();
    // The queue is empty, there's nothing to output.
    return false;
  }
  *stereo_data = queue_stereo_.front();
  queue_stereo_.pop();
  mutex_queue_stereo_.unlock();
  return true;
}
bool PerceptInGen1Interface::GetStereoData(StereoData *stereo_data) {
  CHECK_NOTNULL(stereo_data);

  mutex_streaming_.lock();
  bool streaming = streaming_;
  mutex_streaming_.unlock();
  if (!streaming) {
    return false;
  }

  // Check if the queue is empty.
  mutex_queue_stereo_.lock();
  const bool queue_stereo_empty = queue_stereo_.empty();
  mutex_queue_stereo_.unlock();

  // The queue is empty, there's nothing to output.
  if (queue_stereo_empty) {
    return false;
  }

  // The queue is not empty (and the streaming is still going), pop the latest
  // one in the queue, and then clear the queue.
  std::queue<StereoData> empty;
  mutex_queue_stereo_.lock();
  *stereo_data = queue_stereo_.back();
  std::swap(queue_stereo_, empty);
  mutex_queue_stereo_.unlock();
  return true;
}

bool PerceptInGen1Interface::GetImuData(ImuData *imu_data) {
  CHECK_NOTNULL(imu_data);

  mutex_streaming_.lock();
  bool streaming = streaming_;
  mutex_streaming_.unlock();
  if (!streaming) {
    return false;
  }

  // Check if the queue is empty.
  mutex_queue_imu_.lock();
  const bool queue_imu_empty = queue_imu_.empty();
  mutex_queue_imu_.unlock();

  // The queue is empty, there's nothing to output.
  if (queue_imu_empty) {
    return false;
  }

  // The queue is not empty, pop the latest one in the queue.
  mutex_queue_imu_.lock();
  *imu_data = queue_imu_.front();
  queue_imu_.pop();
  mutex_queue_imu_.unlock();
  return true;
}

/**
 * Decode data in uvc frame into a stereo image.
 */
void DecodeStereoBuffer(uvc_frame_t *frame, cv::Mat * img_left, cv::Mat * img_right) {
  const int WIDTH = frame->width;
  const int HEIGHT = frame->height;
  uint8_t *fptr = reinterpret_cast<uint8_t*>(frame->data);

  // Decode the buffer into a single channel stereo image.
  // Note: Rotating the buffer 180 degree so that the image is up-side-up.
  *img_left = cv::Mat_<uchar>(cv::Size(WIDTH, HEIGHT));
  *img_right = cv::Mat_<uchar>(cv::Size(WIDTH, HEIGHT));
  int k = 0;
  for (int i = HEIGHT - 1; i >= 0; i--) {
    const size_t idx_data_offset = i * WIDTH;
    for (int j = WIDTH - 1; j >= 0; j--) {
      img_right->data[idx_data_offset + j] = static_cast<uchar>(fptr[k++]);
      img_left->data[idx_data_offset + j] = static_cast<uchar>(fptr[k++]);
    }
  }
  return;
}

unsigned long GetTimeInMicroSeconds() {
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return US_TO_SEC * tv.tv_sec + tv.tv_usec;
}

void PerceptInGen1Interface::StreamStereo(uvc_frame_t *frame) {
  // Get time stamp
  unsigned long timestamp = GetTimeInMicroSeconds() / 1000;

  // Decode the buffer and add to queue.
  cv::Mat img_left, img_right;
  DecodeStereoBuffer(frame, &img_left, &img_right);

  mutex_queue_stereo_.lock();
  queue_stereo_.emplace(timestamp, img_left, img_right);
  mutex_queue_stereo_.unlock();
}

imu_data_t GetIMUData(uvc_device_handle_t *devh) {
  imu_data_t result = {0};
  uvc_get_imu_data(devh, &result);
  return result;
}

void PerceptInGen1Interface::StreamImu() {
  while (streaming_ && !stopStreamImuThread) {
    imu_data_t result = GetIMUData(devh_);
    unsigned long timestamp = GetTimeInMicroSeconds() / 1000;

    mutex_queue_imu_.lock();
    queue_imu_.emplace(
        timestamp,
        -(double)result.accelX/32768*19.6,
        -(double)result.accelY/32768*19.6,
        -(double)result.accelZ/32768*19.6,
        -(double)result.gyroX/32768*250*0.0174533,
        -(double)result.gyroY/32768*250*0.0174533,
        -(double)result.gyroZ/32768*250*0.0174533);
    mutex_queue_imu_.unlock();
    usleep(5000);
  }
}

}  // namespace PI
