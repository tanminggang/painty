/**
 * @file PictureTargetSbrPainter.hxx
 * @author Thomas Lindemeier
 * @brief
 * @date 2020-09-11
 *
 */
#pragma once

#include "painty/sbr/SbrPainter.hxx"

namespace painty {
class PictureTargetSbrPainter : public SbrPainter {
 public:
  PictureTargetSbrPainter(const std::shared_ptr<Canvas<vec3>>& canvasPtr,
                          const Palette& basePigmentsPalette);

  auto paintImage(const Mat3d& labImage) -> bool;

 private:
  PictureTargetSbrPainter() = delete;
};
}  // namespace painty
