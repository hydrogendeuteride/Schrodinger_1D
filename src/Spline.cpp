#include "spline.h"

std::vector<Spline> CubicSpline(int n, std::vector<double>& x, std::vector<double>& y)
{
    std::vector<double> h(n), alpha(n), l(n) ,mu(n), z(n);
    std::vector<Spline> spline(n);

    for (int i = 0; i < n - 1; ++i)
    {
        h[i] = x[i + 1] - x[i];
    }

    for (int i = 1; i < n - 1; ++i)
    {
        alpha[i] = 3.0 / h[i] * (y[i + 1] - y[i]) - 3.0 / h[i - 1] * (y[i] - y[i - 1]);
    }
    l[0] = 2.0 * (h[0] + h[1]);
    mu[0] = h[1] / l[0];
    z[0] = alpha[0] / l[0];

    for (int i = 1; i < n - 1; ++i)
    {
        l[i] = 2.0 * (h[i] + h[i + 1]) - h[i-1] * mu[i-1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }
    l[n -1] = h[n - 2];
    z[n -1] = (alpha[n - 1] - h[n - 2] * z[n - 2]) / l[n - 1];
    spline[n - 1].c = z[n - 1];

    for (int i = n - 2; i >= 0; --i)
    {
        spline[i].c = z[i] - mu[i] * spline[i + 1].c;
        spline[i].b = (y[i + 1] - y[i]) / h[i] - h[i] * (spline[i + 1].c + 2.0 * spline[i].c) / 3.0;
        spline[i].d = (spline[i + 1].c - spline[i].c) / (3.0 * h[i]);
        spline[i].a = y[i];
        spline[i].x = x[i];
    }

    return spline;
}

std::vector<Eigen::Vector2d> SplinePoints(int n, int div, std::vector<double>& x, std::vector<double>& y)
{
    const auto tmp = CubicSpline(n, x, y);

    std::vector<Eigen::Vector2d> spline;

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < div; ++j)
        {
            double dx = (x[i + 1] - x[i]) / static_cast<double>(div);
            double cur_x = x[i] + j * dx;
            double delta_x = cur_x - x[i];

            auto t = tmp[i];
            double cur_y = t.a + t.b * delta_x + t.c * delta_x * delta_x + t.d * delta_x * delta_x * delta_x;

            spline.emplace_back(cur_x, cur_y);
        }
    }
    spline.emplace_back(x[n-1], y[n-1]);

    return spline;
}

std::vector<Eigen::Vector2d> LinearSpline(int n, std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size() || x.size() < 2 || n < 2) {
        throw std::runtime_error("Invalid input sizes or number of points");
    }

    std::vector<Eigen::Vector2d> sampledPoints;

    for (size_t i = 0; i < x.size() - 1; ++i) {
        double m = (y[i+1] - y[i]) / (x[i+1] - x[i]);
        double b = y[i] - m * x[i];

        for (int j = 0; j < n; ++j) {
            double fraction = static_cast<double>(j) / (n - 1);
            double xi = x[i] + fraction * (x[i+1] - x[i]);
            double yi = m * xi + b;
            sampledPoints.push_back(Eigen::Vector2d(xi, yi));
        }
    }

    if (sampledPoints.back() != Eigen::Vector2d(x.back(), y.back())) {
        sampledPoints.push_back(Eigen::Vector2d(x.back(), y.back()));
    }

    return sampledPoints;
}