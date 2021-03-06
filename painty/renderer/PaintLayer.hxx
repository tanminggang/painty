/**
 * @file PaintLayer.h
 * @author thomas lindemeier
 *
 * @brief
 *
 * @date 2020-05-14
 *
 */
#pragma once

#include <type_traits>

#include "painty/core/KubelkaMunk.hxx"
#include "painty/image/Mat.hxx"

namespace painty {
/**
 * @brief Stores paint and amount cellwise.
 */
template <class vector_type>
class PaintLayer final {
  using T                 = typename DataType<vector_type>::channel_type;
  static constexpr auto N = DataType<vector_type>::dim;

 public:
  PaintLayer(int32_t rows, int32_t cols)
      : _K_buffer(rows, cols),
        _S_buffer(rows, cols),
        _V_buffer(rows, cols) {}

  const Mat<vector_type>& getK_buffer() const {
    return _K_buffer;
  }

  const Mat<vector_type>& getS_buffer() const {
    return _S_buffer;
  }

  const Mat<T>& getV_buffer() const {
    return _V_buffer;
  }

  Mat<vector_type>& getK_buffer() {
    return _K_buffer;
  }

  Mat<vector_type>& getS_buffer() {
    return _S_buffer;
  }

  Mat<T>& getV_buffer() {
    return _V_buffer;
  }

  int32_t getCols() const {
    return _K_buffer.cols;
  }

  int32_t getRows() const {
    return _K_buffer.rows;
  }

  /**
   * @brief Set all values to zero.
   *
   */
  void clear() {
    for (size_t i = 0; i < _K_buffer.total(); i++) {
      _K_buffer(static_cast<int32_t>(i)).fill(static_cast<T>(0.0));
      _S_buffer(static_cast<int32_t>(i)).fill(static_cast<T>(0.0));
      _V_buffer(static_cast<int32_t>(i)) = static_cast<T>(0.0);
    }
  }

  /**
   * @brief Compose this layer onto a substrate (reflectance). The layer is assumed to be dry.
   *
   * @param R0 the substrate
   */
  void composeOnto(Mat<vector_type>& R0) const {
    if ((R0.rows != _K_buffer.rows) || (R0.cols != _K_buffer.cols)) {
      R0 = Mat<vector_type>(_K_buffer.rows, _K_buffer.cols);
      for (auto& v : R0) {
        v.fill(1.0);
      }
    }

    auto& K       = _K_buffer;
    auto& S       = _S_buffer;
    auto& V       = _V_buffer;
    auto& R0_data = R0;
    for (auto i = 0; i < static_cast<int32_t>(K.total()); i++) {
      R0_data(i) = ComputeReflectance(K(i), S(i), R0_data(i), V(i));
    }
  }

  /**
   * @brief Deep copy this buffer to another.
   *
   * @param other
   */
  void copyTo(PaintLayer& other) const {
    if ((other.getRows() != _K_buffer.rows) ||
        (other.getCols() != _K_buffer.cols)) {
      other = PaintLayer<vector_type>(getRows(), getCols());
    }
    std::copy(_K_buffer.begin(), _K_buffer.end(), other._K_buffer.begin());
    std::copy(_S_buffer.begin(), _S_buffer.end(), other._S_buffer.begin());
    std::copy(_V_buffer.begin(), _V_buffer.end(), other._V_buffer.begin());
  }

  /**
   * @brief Update a cell of the canvas.
   *
   * @param i the row index
   * @param j the col index
   * @param k Scattering
   * @param s Absorption
   * @param v Volume
   */
  void set(int32_t i, int32_t j, const vector_type& k, const vector_type& s,
           const T v) {
    _K_buffer(i, j) = k;
    _S_buffer(i, j) = s;
    _V_buffer(i, j) = v;
  }

 private:
  /**
   * @brief Absorption
   *
   */
  Mat<vector_type> _K_buffer;
  /**
   * @brief Scattering
   *
   */
  Mat<vector_type> _S_buffer;
  /**
   * @brief Amount of paint.
   *
   */
  Mat<T> _V_buffer;
};
}  // namespace painty
