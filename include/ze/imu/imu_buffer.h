#pragma once

#include <ze/imu/imu_model.h>
#include <ze/common/ringbuffer.h>

namespace ze {

template<int BufferSize, typename Interpolator>
class ImuBuffer
{
public:

  ImuBuffer(ImuModel::Ptr imu_model);

  inline void insertGyroscopeMeasurement(time_t stamp, const Vector3);
  inline void insertAccelerometerMeasurement(time_t stamp, const Vector3);

  //! Insert an IMU measurement at a given timestamp: First three values refer
  //! to the accelerometer, last 3 the gyroscope
  void insertImuMeasurement(int64_t time, const Vector6 value);

  //! Get the rectified values of the IMU at a given timestamp. Interpolates
  //! if necessary.
  Vector6 get(int64_t time);

  //! Get all values between two timestamps, synchronize Accelerometer and
  //! Gyroscope, interpolate edges to fit start and end. Interpolates Gyro
  //! and Accel measurements to have equal timestamps.
  std::pair<ImuStamps, ImuAccGyr>
  getBetweenValuesInterpolated(int64_t stamp_from, int64_t stamp_to);

protected:
  Vector3 getAccelerometer(int64_t time);
  Vector3 getGyroscope(int64_t time);

private:
  //! The underlying storage structures for accelerometer and gyroscope
  //! measurements.
  Ringbuffer<FloatType, 3, BufferSize> acc_buffer_;
  Ringbuffer<FloatType, 3, BufferSize> gyr_buffer_;

  ImuModel::Ptr imu_model_;
};

} // namespace ze
