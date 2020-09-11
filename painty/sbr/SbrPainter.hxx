/**
 * @file SbrPainter.hxx
 * @author Thomas Lindemeier
 * @brief
 * @date 2020-09-11
 *
 */
#pragma once

#include "painty/mixer/PaintMixer.hxx"
#include "painty/mixer/Palette.hxx"
#include "painty/renderer/Canvas.hxx"
#include "painty/renderer/FootprintBrush.hxx"

namespace painty {
class SbrPainter final {
 public:
  SbrPainter(const Canvas<vec3>& canvas, const Palette& palette);

  void setBrushRadius(const double radius);

 private:
  SbrPainter() = delete;

  PaintMixer _mixer;

  Canvas<vec3> _canvas;

  FootprintBrush<vec3> _brush;
};
}  // namespace painty
