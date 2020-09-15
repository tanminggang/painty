/**
 * @file PictureTargetSbrPainter.cxx
 * @author Thomas Lindemeier
 * @brief
 * @date 2020-09-11
 *
 */
#include "painty/sbr/PictureTargetSbrPainter.hxx"

#include "painty/core/Color.hxx"
#include "painty/image/Convolution.hxx"
#include "painty/image/EdgeTangentFlow.hxx"
#include "painty/io/ImageIO.hxx"
#include "painty/mixer/Serialization.hxx"
#include "painty/renderer/Renderer.hxx"
#include "painty/sbr/PathTracer.hxx"

namespace painty {
PictureTargetSbrPainter::PictureTargetSbrPainter(
  const std::shared_ptr<Canvas<vec3>>& canvasPtr,
  const std::shared_ptr<PaintMixer>& basePigmentsMixerPtr,
  const std::shared_ptr<BrushBase<vec3>>& painterPtr)
    : _canvasPtr(canvasPtr),
      _basePigmentsMixerPtr(basePigmentsMixerPtr),
      _painterPtr(painterPtr) {}

auto PictureTargetSbrPainter::paint() -> bool {
  if (_paramsInput.inputSRGB.empty()) {
    throw std::runtime_error("_paramsInput.inputSRGB.empty()");
  }
  std::cout << "Converting input to Lab and apply smoothing" << std::endl;
  // convert to CIELab and blur the image using a bilateral filter
  const auto target_Lab = smoothOABF(
    convertColor(_paramsInput.inputSRGB,
                 ColorConverter<double>::Conversion::srgb_2_CIELab),
    Mat1d(), _paramsInput.sigmaSpatial, _paramsInput.sigmaColor,
    _paramsOrientations.outerBlurScale, _paramsInput.smoothIterations);

  painty::io::imSave(
    "/tmp/targetImage.jpg",
    convertColor(target_Lab, ColorConverter<double>::Conversion::CIELab_2_srgb),
    false);

  std::cout << "Extract color palette from image using base pigments"
            << std::endl;
  // mix palette for the image from the painters base pigments
  const auto palette = _basePigmentsMixerPtr->mixFromInputPicture(
    _paramsInput.inputSRGB, _paramsInput.nrColors);
  painty::io::imSave("/tmp/targetImagePalette.jpg",
                     VisualizePalette(palette, 1.0), false);

  // for every brush
  for (const auto brushSize : _paramsStroke.brushSizes) {
    std::cout << "Switching to brush size: " << brushSize << std::endl;
    const double brushRadius = brushSize / 2.0;

    std::cout << "Computing structure tensor field" << std::endl;

    // compute structure tensor field
    const auto tensors =
      tensor::ComputeTensors(target_Lab, _paramsInput.mask,
                             brushRadius * _paramsOrientations.innerBlurScale,
                             brushRadius * _paramsOrientations.outerBlurScale);
    painty::io::imSave("/tmp/targetImageOrientation.jpg",
                       lineIntegralConv(ComputeEdgeTangentFlow(tensors), 10.),
                       false);
    PathTracer tracer(tensors);
    tracer.setMinLen(_paramsStroke.minLen);
    tracer.setMaxLen(_paramsStroke.maxLen);
    tracer.setStep((_paramsStroke.stepSize <= 0.0) ? brushRadius
                                                   : _paramsStroke.stepSize);
    tracer.setFrame(cv::Rect2i(0, 0, target_Lab.cols, target_Lab.rows));
    tracer.setFc(_paramsStroke.curvatureAlpha);

    std::cout << "Iterating layers" << std::endl;

    for (uint32_t iteration = 0U; iteration < _paramsConvergence.maxIterations;
         iteration++) {
      std::cout << "Iteration: " << iteration << std::endl;

      std::cout << "Getting current state of the canvas" << std::endl;
      const auto canvasCurrentRGBLinear = Renderer<vec3>().compose(*_canvasPtr);
      painty::io::imSave("/tmp/canvasCurrent.jpg", canvasCurrentRGBLinear,
                         true);
      const auto canvasCurrentLab =
        convertColor(canvasCurrentRGBLinear,
                     ColorConverter<double>::Conversion::rgb_2_CIELab);

      std::cout << "Compute difference of target and canvas" << std::endl;
      auto difference = Mat1d(target_Lab.size());
      for (auto i = 0; i < static_cast<int32_t>(target_Lab.total()); i++) {
        difference(i) = ColorConverter<double>::ColorDifference(
          target_Lab(i), canvasCurrentLab(i));
      }
      painty::io::imSave("/tmp/difference.jpg", difference, false);

      std::cout << "Extracting superpixels" << std::endl;
      Mat3d segImage = _paramsInput.inputSRGB.clone();
      Mat3d segDiffImage(segImage.size());
      std::map<int32_t, ImageRegion> regions;
      Mat<int32_t> labels;
      {
        SuperpixelSegmentation seg;
        seg.setUseDiffWeight(_paramsRegionExtraction.useDiffWeights);
        seg.setExtractionStrategy(_paramsRegionExtraction.extractionStrategy);
        seg.extract(target_Lab, canvasCurrentLab, _paramsInput.mask,
                    static_cast<int32_t>(brushSize));
        labels = seg.getRegions(regions);
        for (auto j = 0; j < static_cast<int32_t>(segDiffImage.total()); ++j) {
          segDiffImage(j)[0] = difference(j);
          segDiffImage(j)[1] = difference(j);
          segDiffImage(j)[2] = difference(j);
        }
        seg.getSegmentationOutlined(segImage);
        seg.getSegmentationOutlined(segDiffImage);
        painty::io::imSave("/tmp/superpixelsTarget.jpg", segImage, false);
        painty::io::imSave("/tmp/superpixelsDifference.jpg", segDiffImage,
                           false);
      }
    }
  }

  return true;
}

}  // namespace painty
