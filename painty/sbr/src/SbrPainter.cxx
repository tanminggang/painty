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
      _canvas(canvas) {}

}  // namespace painty
