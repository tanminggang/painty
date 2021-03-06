/**
 * @file Superpixel.h
 * @author Thomas Lindemeier
 * @brief
 *
 * @date 2020-07-31
 *
 *
 */
#pragma once

#include <map>
#include <vector>

#include "painty/image/Mat.hxx"

namespace painty {
class ImageRegion {
  int32_t label;

  std::vector<vec2i> points;

  bool active;

 public:
  ImageRegion();

  ImageRegion(int32_t label, const Mat1i& labelsMap);

  void setLabel(int32_t label);

  void setActive(bool active);

  bool isActive() const;

  int32_t getLabel() const;

  vec2 getSpatialMean() const;

  std::vector<vec2i>::const_iterator cbegin();

  std::vector<vec2i>::const_iterator cend();

  double getInscribedCircle(vec2& incenter) const;

  template <class T>
  typename DataType<T>::channel_type computeRms(const Mat<T>& data0,
                                                const Mat<T>& data1) const {
    if (points.empty()) {
      return T(0.);
    }
    typename DataType<T>::channel_type s = T(0.);
    for (const vec2i& p : points) {
      s += (data0(p[1], p[0]) - data1(p[1], p[0])).squaredNorm();
    }

    return std::sqrt(s /= points.size());
  }

  template <class T>
  T computeRms(const Mat<T>& diff) const {
    if (points.empty()) {
      return T(0.);
    }
    T s = T(0.);
    for (const vec2i& p : points) {
      s += std::pow(diff(p[1], p[0]), 2.);
    }

    return std::sqrt(s /= points.size());
  }

  template <class T>
  void fill(Mat<T>& m, const T& v) const {
    if (points.empty()) {
      return;
    }
    for (const vec2i& p : points) {
      m(p[1], p[0]) = v;
    }
  }

  template <class T>
  T computeSum(const Mat<T>& v) const {
    if (points.empty()) {
      return T(0.);
    }
    T s = T(0.);
    for (const vec2i& p : points) {
      s += v(p[1], p[0]);
    }

    return s;
  }

  template <class T>
  T computeMean(const Mat<T>& data) const {
    T m = 0.;
    for (const vec2i& p : points) {
      m += data(p[1], p[0]);
    }
    return (1.0 / static_cast<double>(points.size())) * m;
  }

  template <class T, int32_t Channels>
  vec<T, Channels> computeMean(const Mat<vec<T, Channels> >& data) const {
    vec<T, Channels> m = vec<T, Channels>::Zero();
    for (const vec2i& p : points) {
      m += data(p[1], p[0]);
    }
    return (1.0 / points.size()) * m;
  }

  Mat1d getDistanceTransform(const cv::Rect2i& boundingRectangle) const;

  cv::Rect2i getBoundingRectangle() const;
};

class SuperpixelSegmentation {
  class SuperPixel {
   public:
    vec2 _center;
    vec2 _centerT;
    vec3 _meanColor;
    vec3 _meanColorT;
    double _meanDiff;
    double _meanDiffT;
    int32_t _area;

    double _maxSpatialDist;
    double _maxSpatialDistT;
    double _maxColorDist;
    double _maxColorDistT;
    double _maxDiffDist;
    double _maxDiffDistT;

    void reset();

    SuperPixel();

    SuperPixel(const vec2& center, const vec3& meanColor);

    vec2 getCenter() const {
      return _center;
    }

    vec3 getMeanColor() const {
      return _meanColor;
    }

    int32_t getArea() const {
      return _area;
    }
  };

 public:
  enum ExtractionStrategy : uint8_t {
    SLICO_GRID             = static_cast<uint8_t>(0U),
    SLICO_POISSON_WEIGHTED = static_cast<uint8_t>(1U),
    GRID_SHUFFLED          = static_cast<uint8_t>(2U)
  };

  void extract(const Mat3d& targetLab, const Mat3d& canvasLab,
               const Mat<uint8_t>& mask, int32_t cellWidth);

  void getSegmentationOutlined(Mat3d& background) const;

  const Mat1i& getRegions(std::map<int32_t, ImageRegion>& regions);

  void setExtractionStrategy(ExtractionStrategy extractionStrategy);

  void setUseDiffWeight(bool useDiffWeight);

 private:
  void perturbClusterCenters(std::vector<SuperPixel>& superPixels) const;

  double computeStats(std::vector<SuperPixel>& superPixels,
                      Mat<int32_t>& labels) const;

  double distance(SuperPixel& superPixel, const vec2i& pos2) const;

  std::vector<SuperPixel> _superPixels;
  Mat3d _targetLab;
  Mat1d _difference;
  Mat<int32_t> _labels;
  Mat<uint8_t> _mask;

  ExtractionStrategy _extractionStrategy =
    ExtractionStrategy::SLICO_POISSON_WEIGHTED;

  bool _useDiffWeight = true;
};

}  // namespace painty
