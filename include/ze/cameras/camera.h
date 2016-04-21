#pragma once

#include <string>
#include <memory>
#include <ze/common/logging.hpp>
#include <Eigen/Core>

#include <imp/core/image.hpp>
#include <imp/core/size.hpp>
#include <ze/common/macros.h> 
#include <ze/common/types.h>

namespace ze {

enum class CameraType {
  Pinhole = 0,
  PinholeFov = 1,
  PinholeEquidistant = 2,
  PinholeRadialTangential = 3
};

class Camera
{
public:
  ZE_POINTER_TYPEDEFS(Camera);

public:

  Camera(const uint32_t width, const uint32_t height, const CameraType type,
         const VectorX& projection_params, const VectorX& distortion_params);

  virtual ~Camera() = default;

  //! Load a camera rig form a yaml file. Returns a nullptr if the loading fails.
  static Ptr loadFromYaml(const std::string& path);

  //! Vearing vector from pixel coordinates. Z-component of return value is 1.0.
  virtual Bearing backProject(const Eigen::Ref<const Keypoint>& px) const = 0;

  //! Computes pixel coordinates from 3D-point.
  virtual Keypoint project(const Eigen::Ref<const Position>& pos) const = 0;

  //! Computes Jacobian of projection w.r.t. bearing vector.
  virtual Matrix23 dProject_dLandmark(const Eigen::Ref<const Position>& pos) const = 0;

  //! @name Block operations. Always prefer these to avoid cache misses.
  //!@{
  virtual Bearings backProjectVectorized(const Eigen::Ref<const Keypoints>& px_vec) const;
  virtual Keypoints projectVectorized(const Eigen::Ref<const Bearings>& bearing_vec) const;
  virtual Matrix6X dProject_dLandmarkVectorized(const Positions& pos_vec) const;
  //!@}

  //! @name Image dimension.
  //! @{
  inline uint32_t width() const { return size_.width(); }
  inline uint32_t height() const { return size_.height(); }
  inline Size2u size() const { return size_; }
  //! @}

  //! CameraType value representing the camera model used by the derived class.
  inline CameraType type() const { return type_; }

  //! CameraType as descriptive string.
  std::string typeAsString() const;

  //! Camera projection parameters.
  inline const VectorX& projectionParameters() const { return projection_params_; }

  //! Camera distortion parameters.
  inline const VectorX& distortionParameters() const { return distortion_params_; }

  //! Name of the camera.
  inline const std::string& label() const { return label_; }

  //! Set user-specific camera label.
  inline void setLabel(const std::string& label) { label_ = label; }

  //! Get angle corresponding to one pixel in image plane.
  //! @todo: make static cache.
  virtual FloatType getApproxAnglePerPixel() const = 0;

  //! Set mask: 0 = masked, >0 = unmasked.
  void setMask(const Image8uC1::Ptr& mask);

  //! Get mask.
  inline Image8uC1::ConstPtr mask() const { return mask_; }

protected:

  Size2u size_;

  //! Camera projection parameters, e.g., (fx, fy, cx, cy).
  VectorX projection_params_;

  //! Camera distortion parameters, e.g., (k1, k2, r1, r2).
  VectorX distortion_params_;

  //! Camera distortion parameters
  std::string label_;
  CameraType type_;

  //! Mask
  Image8uC1::Ptr mask_ = nullptr;
};

//! Print camera:
std::ostream& operator<<(std::ostream& out, const Camera& cam);

} // namespace ze
