/**
 * @file math.h
 * @author thomas lindemeier
 *
 * @brief
 *
 * @date 2020-04-22
 *
 */
#ifndef PAINTY_MATH_H
#define PAINTY_MATH_H

#include <cmath>
#include <type_traits>
#include <vector>
#include <stdexcept>

#include <painty/vec.h>

namespace painty
{
constexpr auto PI = 3.141592653589793238462643383279502884197169399375105820974;

/**
 * @brief Fuzzy comparison of floating points.
 *
 * @tparam Float the floating point type.
 * @param firstValue first value for comparison.
 * @param secondValue second value for comparison.
 * @param epsilon
 * @return if the given values are fuzzy equal.
 */
template <typename Float>
typename std::enable_if<std::is_floating_point<Float>::value, bool>::type fuzzyCompare(const Float firstValue,
                                                                                       const Float secondValue,
                                                                                       const Float epsilon)
{
  return std::fabs(firstValue - secondValue) < epsilon;
}

/**
 * @brief Fuzzy comparison of floating points.
 *
 * @tparam Float the floating point type.
 * @param firstValue first value for comparison.
 * @param secondValue second value for comparison.
 * @param epsilon
 *
 * @return if the given values are fuzzy equal.
 */
template <typename Float, size_t N>
typename std::enable_if<std::is_floating_point<Float>::value, bool>::type
fuzzyCompare(const std::array<Float, N>& firstValue, const std::array<Float, N>& secondValue, const Float epsilon)
{
  auto equal = true;
  for (size_t i = 0U; ((i < N) && equal); i++)
  {
    equal = fuzzyCompare<Float>(firstValue[i], secondValue[i], epsilon);
  }
  return equal;
}

/**
 * @brief https://www.in.tu-clausthal.de/fileadmin/homes/techreports/ifi0505hormann.pdf Generalized barycentric
 * coordinates for arbitrary polygons
 *
 * @tparam Value
 * @param polygon list of 2d points in clock or anticlock wise order
 * @param position the 2d position to interpolate a values
 * @param values the list of values along the polygon
 *
 * @return Value the interpolated value at v
 */
template <class Value>
Value generalizedBarycentricCoordinatesInterpolate(const std::vector<vec2>& polygon, const vec2& position,
                                                   const std::vector<Value>& values)
{
  const auto n = polygon.size();

  constexpr auto Eps = std::numeric_limits<double>::epsilon() * 100.0;

  if (polygon.empty() || values.empty())
  {
    throw std::invalid_argument("Polygon is empty");
  }
  else if (polygon.size() != values.size())
  {
    throw std::invalid_argument("Polygon size differs from values size");
  }
  else if (n == 1U)
  {
    return values.front();
  }

  std::vector<vec2> s(n);
  for (size_t i = 0U; i < n; ++i)
  {
    s[i] = polygon[i] - position;
  }
  std::vector<double> r(n);
  std::vector<double> A(n);
  std::vector<double> D(n);
  for (size_t i = 0U; i < n; ++i)
  {
    vec2 si1 = (i == n - 1) ? s[0] : s[i + 1];
    vec2 si = s[i];

    r[i] = norm(si);

    if (fuzzyCompare(r[i], 0.0, Eps))
    {
      return values[i];
    }

    /*
    |si.x si1.x|
    |si.y si1.y|
    */
    const auto det = si[0] * si1[1] - si1[0] * si[1];
    A[i] = det / 2.0;
    D[i] = dot(si, si1);

    if (fuzzyCompare(A[i], 0.0, Eps) && D[i] < 0.0)
    {
      double& ri1 = (i == n - 1) ? r[0] : r[i + 1];
      Value fi1 = (i == n - 1) ? values.front() : values[i + 1];
      ri1 = norm(si1);
      return (ri1 * values[i] + r[i] * fi1) * (1.0 / (r[i] + ri1));
    }
  }

  Value f = {};
  double W = 0.;

  for (size_t i = 0U; i < n; ++i)
  {
    double w = 0.;
    double A_1 = (i == 0) ? A.back() : A[i - 1];

    if (A_1 != 0.)
    {
      double r_1 = (i == 0) ? r.back() : r[i - 1];
      double D_1 = (i == 0) ? D.back() : D[i - 1];

      w = w + (r_1 - D_1 / r[i]) / A_1;
    }
    if (A[i] != 0.)
    {
      double ri1 = (i == n - 1) ? r[0] : r[i + 1];

      w = w + (ri1 - D[i] / r[i]) / A[i];
    }
    f = f + w * values[i];
    W = W + w;
  }
  if (!fuzzyCompare(W, 0.0, Eps))
  {
    return f * (1.0 / W);
  }
  else
  {
    return values.front();
  }
}

/**
 * @brief Cotangent hyperbolicus
 *
 * @tparam Float
 * @tparam 0
 * @param x
 * @return Float
 */
template <typename Float, typename std::enable_if_t<std::is_floating_point<Float>::value, int> = 0>
Float coth(const Float& x)
{
  if (x > static_cast<Float>(20.0))
  {
    return static_cast<Float>(1.0);
  }
  else
  {
    if (std::fabs(x) > static_cast<Float>(0.0))
    {
      const auto res = std::cosh(x) / std::sinh(x);
      return std::isnan(res) ? static_cast<Float>(1.0) : res;
    }
    else
    {
      return std::numeric_limits<Float>::infinity();
    }
  }
}

template <typename Float, size_t N, typename std::enable_if_t<std::is_floating_point<Float>::value, int> = 0>
vec<Float, N> coth(const vec<Float, N>& x)
{
  vec<Float, N> res;
  for (auto i = 0U; i < N; i++)
  {
    res[i] = coth(x[i]);
  }
}

}  // namespace painty

#endif  // PAINTY_MATH_H
