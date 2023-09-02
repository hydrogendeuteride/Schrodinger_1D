#ifndef SCHRODINGER_1D_SPLINE_H
#define SCHRODINGER_1D_SPLINE_H

#include <utility>
#include <vector>
#include <eigen3/Eigen/Dense>

struct Spline
{
    double a, b, c, d, x;
};

std::vector<Spline> CubicSpline(int n, std::vector<double>& x, std::vector<double>& y);


std::vector<Eigen::Vector2d> SplinePoints(int n, int div, std::vector<double>& x, std::vector<double>& y);

std::vector<Eigen::Vector2d> LinearSpline(int n, std::vector<double>& x, const std::vector<double>& y);

#endif //SCHRODINGER_1D_SPLINE_H
