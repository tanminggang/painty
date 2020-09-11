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
class SbrPainter {
 public:
  SbrPainter(const std::shared_ptr<Canvas<vec3>>& canvasPtr,
             const Palette& basePigmentsPalette);

  void setBrushRadius(const double radius);

  void paintStroke(const std::vector<vec2>& path);

  void dipBrush(const std::array<vec3, 2UL>& paint);

  auto getMixer() const -> const PaintMixer&;

 private:
  SbrPainter() = delete;

  PaintMixer _mixer;

  std::shared_ptr<Canvas<vec3>> _canvasPtr = nullptr;

  FootprintBrush<vec3> _brush;
};
}  // namespace painty
