/**
 * @file SbrPainter.cxx
 * @author Thomas Lindemeier
 * @brief
 * @date 2020-09-11
 *
 */
#include "painty/sbr/SbrPainter.hxx"

namespace painty {

SbrPainter::SbrPainter(const Canvas<vec3>& canvas, const Palette& palette)
    : _mixer(palette),
      _canvas(canvas),
      _brush(0.0) {}

void SbrPainter::setBrushRadius(const double radius) {
  _brush.setRadius(radius);
}

}  // namespace painty
