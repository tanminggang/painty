/**
 * @file ImageIO.h
 * @author thomas lindemeier
 *
 * @brief
 *
 * @date 2020-04-22
 *
 */
#pragma once

#include "painty/core/Vec.h"
#include "painty/image/Mat.h"

namespace painty {
namespace io {
void imRead(const std::string& filename, Mat<vec3>& linear_rgb,
            bool convertFrom_sRGB);

void imRead(const std::string& filename, Mat<double>& gray,
            bool convertFrom_sRGB);

bool imSave(const std::string& filename, const Mat<vec3>& linear_rgb,
            bool convertTo_sRGB);

bool imSave(const std::string& filename, const Mat<double>& gray,
            bool convertTo_sRGB);
}  // namespace io

}  // namespace painty
