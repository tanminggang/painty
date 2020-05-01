/**
 * @file vec_test.cpp
 * @author thomas lindemeier
 *
 * @brief
 *
 * @date 2020-04-22
 *
 */

#include <gtest/gtest.h>

#include <painty/brush_stroke_sample.h>

TEST(BrushStrokeSample, Construct)
{
  const auto sample = painty::BrushStrokeSample();

  EXPECT_EQ(800U, sample.getThicknessMap().getCols());
  EXPECT_EQ(171U, sample.getThicknessMap().getRows());

  EXPECT_NEAR(0.58, sample.getSampleAt({ 341, 101 }), 0.01);
}
