#ifndef SCHRODINGER_1D_SPLINE_H
#define SCHRODINGER_1D_SPLINE_H

#include <utility>
#include <vector>
#include <eigen3/Eigen/Dense>

struct Spline
{
    double a, b, c, d, x;
};

std::vector<Spline> CubicSpline(int n, std::vector<double>& x, std::vector<double>& y);//use SplinePoints don't use this function

std::vector<Eigen::Vector2d> SplinePoints(int n, int div, std::vector<double>& x, std::vector<double>& y);//return 2d cubic spline of points

std::vector<Eigen::Vector2d> LinearSpline(int n, std::vector<double>& x, const std::vector<double>& y);//simple Linear spline implement

#endif //SCHRODINGER_1D_SPLINE_H
