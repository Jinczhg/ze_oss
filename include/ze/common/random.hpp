#pragma once

#include <random>
#include <ze/common/logging.hpp>
#include <ze/common/types.h>

namespace ze {

//------------------------------------------------------------------------------
//! @return Sample from integer-valued distribution.
template<typename T>
T sampleUniformIntDistribution(
    bool deterministic = false,
    T from = std::numeric_limits<T>::lowest(),
    T to   = std::numeric_limits<T>::max())
{
  static std::mt19937 gen_nondeterministic(std::random_device{}());
  static std::mt19937 gen_deterministic(0);
  auto dist = std::uniform_int_distribution<T>(from, to);
  return deterministic ? dist(gen_deterministic) : dist(gen_nondeterministic);
}

//------------------------------------------------------------------------------
//! @return Sample from uniform real-valued distribution.
template<typename T>
T sampleUniformRealDistribution(
    bool deterministic = false,
    T from = T{0.0},
    T to   = T{1.0})
{
  static std::mt19937 gen_nondeterministic(std::random_device{}());
  static std::mt19937 gen_deterministic(0);
  auto dist = std::uniform_real_distribution<T>(from, to);
  return deterministic ? dist(gen_deterministic) : dist(gen_nondeterministic);
}

//------------------------------------------------------------------------------
//! @return Sample from normal distribution (real-valued).
template<typename T>
T sampleNormalDistribution(
    bool deterministic = false,
    T mean  = T{0.0},
    T sigma = T{1.0})
{
  static std::mt19937 gen_nondeterministic(std::random_device{}());
  static std::mt19937 gen_deterministic(0);
  auto dist = std::normal_distribution<T>(mean, sigma);
  return deterministic ? dist(gen_deterministic) : dist(gen_nondeterministic);
}

//------------------------------------------------------------------------------
//! @return Return true with given probability. Samples the Bernoulli distribution.
inline bool flipCoin(
    bool deterministic = false,
    real_t true_probability = real_t{0.5})
{
  DEBUG_CHECK_GE(true_probability, 0.0);
  DEBUG_CHECK_LT(true_probability, 1.0);
  static std::mt19937 gen_nondeterministic(std::random_device{}());
  static std::mt19937 gen_deterministic(0);
  auto dist = std::bernoulli_distribution(true_probability);
  return deterministic ? dist(gen_deterministic) : dist(gen_nondeterministic);
}

//------------------------------------------------------------------------------
// Sample manifolds:

//! @return Random 3-dimensional unit vector.
Vector3 randomDirection3D();

//! @return Random 2-dimensional unit vector.
Vector2 randomDirection2D();

// -----------------------------------------------------------------------------
// Get distributions, only slightly faster than the above functions when many
// random numbers are desired.

//! Usage: f = uniformDistribution<int>(); sample = f();
//! @return Uniform integer distribution in interval [from, to].
template<class T>
typename std::enable_if<std::is_integral<T>::value, std::function<T()> >::type
uniformDistribution(
    bool deterministic = false,
    T from = std::numeric_limits<T>::lowest(),
    T to   = std::numeric_limits<T>::max())
{
  static std::mt19937 gen_nondeterministic(std::random_device{}());
  static std::mt19937 gen_deterministic(0);
  std::uniform_int_distribution<T> distribution(from, to);
  auto fun = deterministic ?
               std::bind(distribution, gen_deterministic) :
               std::bind(distribution, gen_nondeterministic);
  return fun;
}

// -----------------------------------------------------------------------------
//! Usage: f = uniformDistribution<double>(); sample = f();
//! @return Uniform real-valued distribution in interval [from, to].
template<class T>
typename std::enable_if<!std::is_integral<T>::value, std::function<T()> >::type
uniformDistribution(
    bool deterministic = false,
    T from = T{0.0},
    T to   = T{1.0})
{
  static std::mt19937 gen_nondeterministic(std::random_device{}());
  static std::mt19937 gen_deterministic(0);
  std::uniform_real_distribution<T> distribution(from, to);
  auto fun = deterministic ?
               std::bind(distribution, gen_deterministic) :
               std::bind(distribution, gen_nondeterministic);
  return fun;
}

// -----------------------------------------------------------------------------
//! Usage: f = uniformDistribution<double>(); sample = f();
//! @return Uniform real-valued distribution in interval [from, to].
template<class T>
std::function<T()>
normalDistribution(
    bool deterministic = false,
    T mean  = T{0.0},
    T sigma = T{1.0})
{
  static std::mt19937 gen_nondeterministic(std::random_device{}());
  static std::mt19937 gen_deterministic(0);
  std::normal_distribution<T> distribution(mean, sigma);
  auto fun = deterministic ?
               std::bind(distribution, gen_deterministic) :
               std::bind(distribution, gen_nondeterministic);
  return fun;
}

} // namespace ze
