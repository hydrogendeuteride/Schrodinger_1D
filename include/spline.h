#ifndef SCHRODINGER_1D_SPLINE_H
#define SCHRODINGER_1D_SPLINE_H

#include <vector>

struct Spline
{
    double a, b, c, d, x;
};

std::vector<Spline> CubicSpline(int n, std::vector<double> x, std::vector<double> y)
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
        l[i] = 2.0 * (x[i + 1] - x[i - 1]) - h[i-1] * mu[i-1];
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
        spline[i].d = (spline[i + 1].c = spline[i].c) / (3.0 * h[i]);
        spline[i].a = y[i];
        spline[i].x = x[i];
     }

    return spline;
}

#endif //SCHRODINGER_1D_SPLINE_H
