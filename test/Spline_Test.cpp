#include <gtest/gtest.h>
#include "spline.h"

TEST(SplineTest, SplineTest)
{
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> y = {0.0, 0.8, 0.9, 0.1, -0.8, -1.0};

    auto tmp = CubicSpline(6, x, y);

    std::vector<double> a = {0.0, 0.8, 0.9, 0.1, -0.8};
    std::vector<double> b = {0.87, 0.54, -0.35, -1.2, -1.0};
    std::vector<double> c = {0.1, -0.43, -0.46, -0.42, 1.8};
    std::vector<double> d = {-0.18, -0.0, 0.0, 0.7, -1.0};

    for (int i = 0; i < 5; ++i)
    {
        EXPECT_NEAR(tmp[i].a, a[i], 0.1);
        EXPECT_NEAR(tmp[i].b, b[i], 0.1);
        EXPECT_NEAR(tmp[i].c, c[i], 0.1);
        EXPECT_NEAR(tmp[i].d, d[i], 0.1);
    }
}