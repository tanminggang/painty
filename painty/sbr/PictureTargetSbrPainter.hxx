/**
 * @file PictureTargetSbrPainter.hxx
 * @author Thomas Lindemeier
 * @brief
 * @date 2020-09-11
 *
 */
#pragma once

#include <memory>

#include "painty/image/Superpixel.hxx"
#include "painty/mixer/PaintMixer.hxx"
#include "painty/renderer/BrushBase.hxx"
#include "painty/renderer/Canvas.hxx"

namespace painty {
class PictureTargetSbrPainter {
 public:
  struct ParamsInput {
    Mat3d inputSRGB;  // target image
    Mat1d mask;       // mask for marking areas that should be skipped
    double sigmaSpatial       = 3.0;  // bilateral filter spatial sigma
    double sigmaColor         = 4.25;  // bilateral filter color sigma
    uint32_t smoothIterations = 5U;   // bilateral filter color sigma
    uint32_t nrColors         = 6U;
  };

  struct ParamsOrientations {
    double innerBlurScale =
      0.0;  // Gaussian blur scale for orientation derivatives
    double outerBlurScale = 1.0;  // Gaussian blur scale for orientation tensors
  };

  struct ParamsStroke {
    std::vector<double> brushSizes = {30.0, 20.0, 10.0, 5.0};
    uint32_t minLen                = 5U;  // min number of stroke control points
    uint32_t maxLen = 10U;                // max number of stroke control points
    double stepSize =
      0.0;  // step size used for tracing stroke paths ( <= 0) for step size computed from brush radius
    double curvatureAlpha =
      1.0;  // stroke smoothness (0 for straight, 1 for smooth curved strokes)
    bool blockVisitedRegions =
      true;  // block regions from stroke seeding when the have been visited by another, previously generated stroke
    bool clampBrushRadius = true;  //
    double orientationSmoothAlpha =
      0.0;  // 0 for straight orientations-> all orientation control point are set to the mean orientation
  };

  struct ParamsConvergence {
    uint32_t maxIterations = 5U;  // maximum number of layers
    double rms_local =
      0.1;  // maximum root mean square error for evaluation regions
    double rms_global = 0.0;  // maximum root mean square error the whole image
  };

  struct ParamsPaintSequence {
    double dipAfter =
      400.0;  // maximum length of painting movement before the brush will be dipped in paint again
  };

  struct ParamsRegionExtraction {
    bool useDiffWeights = true;
    SuperpixelSegmentation::ExtractionStrategy extractionStrategy =
      SuperpixelSegmentation::ExtractionStrategy::SLICO_POISSON_WEIGHTED;
  };

   PictureTargetSbrPainter(
     const std::shared_ptr<Canvas<vec3>>& canvasPtr,
     const std::shared_ptr<PaintMixer>& basePigmentsMixerPtr,
     const std::shared_ptr<BrushBase<vec3>>& painterPtr);

   auto paint() -> bool;

   PictureTargetSbrPainter() = delete;

   ParamsInput _paramsInput;
   ParamsOrientations _paramsOrientations;
   ParamsStroke _paramsStroke;
   ParamsConvergence _paramsConvergence;
   ParamsPaintSequence _paramsPaintSequence;
   ParamsRegionExtraction _paramsRegionExtraction;


  private:
   std::shared_ptr<Canvas<vec3>> _canvasPtr          = nullptr;
   std::shared_ptr<PaintMixer> _basePigmentsMixerPtr = nullptr;
   std::shared_ptr<BrushBase<vec3>> _painterPtr      = nullptr;
};
}  // namespace painty
