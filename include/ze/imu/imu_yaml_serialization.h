#pragma once

#include <memory>

#include <ze/imu/accelerometer_model.h>
#include <ze/imu/gyroscope_model.h>
#include <ze/common/logging.hpp>
#include <ze/common/transformation.h>
#include <yaml-cpp/yaml.h>

namespace ze {
 class ImuModel;
 class ImuRig;
}

namespace YAML {

template<>
struct convert<std::shared_ptr<ze::ImuModel>>
{
  static bool decode(const Node& node,  std::shared_ptr<ze::ImuModel>& imu);
  static Node encode(const std::shared_ptr<ze::ImuModel>& imu);
};

template<>
struct convert<std::shared_ptr<ze::ImuRig>>
{
  static bool decode(const Node& node,  std::shared_ptr<ze::ImuRig>& imu);
  static Node encode(const std::shared_ptr<ze::ImuRig>& imu);
};

struct internal
{
  static typename std::shared_ptr<ze::ImuIntrinsicModel> decodeIntrinsics(
      const Node& node);

  static typename std::shared_ptr<ze::ImuNoiseModel> decodeNoise(
      const Node& node);

  static bool validateNoise(const std::string& value);
  static bool validateIntrinsic(const std::string& value);
};

}  // namespace YAML
