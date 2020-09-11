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

namespace painty {
class SbrPainter final {
 public:
  SbrPainter(const Canvas<vec3>& canvas, const Palette& palette);

 private:
  SbrPainter() = delete;

  PaintMixer _mixer;

  Canvas<vec3> _canvas;
};
}  // namespace painty
