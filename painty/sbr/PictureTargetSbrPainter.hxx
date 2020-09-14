/**
 * @file PictureTargetSbrPainter.hxx
 * @author Thomas Lindemeier
 * @brief
 * @date 2020-09-11
 *
 */
#pragma once

#include <memory>

#include "painty/mixer/PaintMixer.hxx"
#include "painty/renderer/BrushBase.hxx"
#include "painty/renderer/Canvas.hxx"

namespace painty {
class PictureTargetSbrPainter {
 public:
  PictureTargetSbrPainter(
    const std::shared_ptr<Canvas<vec3>>& canvasPtr,
    const std::shared_ptr<PaintMixer>& basePigmentsMixerPtr,
    const std::shared_ptr<BrushBase<vec3>>& painterPtr);

  auto paintImage(const Mat3d& labImage) -> bool;

 private:
  PictureTargetSbrPainter() = delete;

  std::shared_ptr<Canvas<vec3>> _canvasPtr          = nullptr;
  std::shared_ptr<PaintMixer> _basePigmentsMixerPtr = nullptr;
  std::shared_ptr<BrushBase<vec3>> _painterPtr      = nullptr;
};
}  // namespace painty
