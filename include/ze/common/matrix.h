// Copyright (C) ETH Zurich, Wyss Zurich, Zurich Eye - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

#pragma once

#include <algorithm>
#include <tuple>
#include <ze/common/logging.hpp>

#include <ze/common/types.h>

namespace ze {

// ----------------------------------------------------------------------------
//! Skew symmetric matrix.
inline Matrix3 skewSymmetric(const Eigen::Ref<const Vector3>& w)
{
  return (Matrix3() <<
           0.0f, -w(2),  w(1),
           w(2),  0.0f, -w(0),
          -w(1),  w(0),  0.0f).finished();
}
inline Matrix3 skewSymmetric(const real_t w1,
                             const real_t w2,
                             const real_t w3)
{
  return (Matrix3() <<
           0.0f, -w3,  w2,
           w3,  0.0f, -w1,
          -w2,  w1,  0.0f).finished();
}

// ----------------------------------------------------------------------------
//! Normalize a block of bearing vectors.
inline void normalizeBearings(Bearings& bearings)
{
  bearings = bearings.array().rowwise() / bearings.colwise().norm().array();
}

// ----------------------------------------------------------------------------
template<typename Derived>
Eigen::Matrix<typename Derived::Scalar, 2, 1> project2(const Eigen::MatrixBase<Derived>& v)
{
  EIGEN_STATIC_ASSERT_MATRIX_SPECIFIC_SIZE(Derived, 3, 1);
  return v.template head<2>() / v(2);
}

// ----------------------------------------------------------------------------
inline Matrix2X project2Vectorized(const Matrix3X& v)
{
  Matrix2X m(2, v.cols());
  for (int i = 0; i < v.cols(); ++i)
  {
    m.col(i) = v.block<2,1>(0,i) / v(2,i);
  }
  return m;
}

// ----------------------------------------------------------------------------
//! Get element with max norm in a vector.
inline real_t normMax(const VectorX& v)
{
  return v.lpNorm<Eigen::Infinity>();
}

// ----------------------------------------------------------------------------
//! Get element with maximum norm on diagonal.
template<typename Derived>
real_t maxAbsDiagonalElement(const Eigen::MatrixBase<Derived>& M)
{
  real_t max_val = 0.0f;
  CHECK_EQ(M.cols(), M.rows());
  for (int i = 0; i < M.cols(); ++i)
  {
    max_val = std::max(max_val, std::abs(M(i,i)));
  }
  return max_val;
}

// ----------------------------------------------------------------------------
//! Direct linear transform algorithm that calls svd to find a vector v that
//! minimizes the algebraic error A*v
//! @param A of size m*n, where m>=n (pad with zero rows if not!)
//! @return Rank of A, minimum error (singular value), and corresponding
//! eigenvector (column of V, with A=U*S*V')
std::tuple<int, real_t, VectorX> directLinearTransform(
    const MatrixX& A, real_t rank_tol = 1e-9);

// ----------------------------------------------------------------------------
//! Get a slice of vector X by the specified indices.
template<typename Derived>
Eigen::Matrix<typename Derived::Scalar, Eigen::Dynamic, 1> getVectorElements(
    Eigen::MatrixBase<Derived>& X, const std::vector<uint32_t>& indices)
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived);
  const uint32_t n = indices.size();
  Eigen::Matrix<typename Derived::Scalar, Eigen::Dynamic, 1> Y(n);
  for (uint32_t i = 0; i < n; ++i)
  {
    Y(i) = X(indices[i]);
  }
  return Y;
}

// ----------------------------------------------------------------------------
//! Get a slice of the matrix X by the specified column indices.
template<typename Derived>
Eigen::Matrix<typename Derived::Scalar, Eigen::Dynamic, Eigen::Dynamic> getMatrixCols(
    Eigen::MatrixBase<Derived>& X, const std::vector<uint32_t>& column_indices)
{
  const uint32_t n_col = column_indices.size();
  Eigen::Matrix<typename Derived::Scalar, Eigen::Dynamic, Eigen::Dynamic> Y(X.rows(), n_col);
  for (uint32_t i = 0; i < n_col; ++i)
  {
    Y.col(i) = X.col(column_indices[i]);
  }
  return Y;
}

} // namespace ze
