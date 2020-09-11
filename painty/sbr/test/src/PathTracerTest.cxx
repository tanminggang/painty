/**
 * @file PathTracerTest.cxx
 * @author thomas lindemeier
 *
 * @brief
 *
 * @date 2020-09-11
 *
 */

#include "gtest/gtest.h"
#include "painty/sbr/PathTracer.hxx"

TEST(PathTracerTest, Construct) {
  painty::Mat3d tensors(1000, 1000);
  for (auto& t : tensors) {
    t = {1.0, 0.0, 0.0};
  }

  auto tracer = painty::PathTracer(tensors);

  tracer.setMinLen(3);
  tracer.setMaxLen(14);
  tracer.setStep(5);
  tracer.setFc(0.5);

  const auto path = tracer.trace({500.0, 500.0});

  EXPECT_EQ(path.size(), 14U);
}
