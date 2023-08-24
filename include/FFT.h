#ifndef SCHRODINGER_1D_FFT_H
#define SCHRODINGER_1D_FFT_H

#include <cmath>
#include <complex>
#include <vector>
#include <optional>

const double PI = std::acos(-1);
namespace FFT
{
    void FFT(std::vector<std::complex<double>> &a);

    void IFFT(std::vector<std::complex<double>> &a);

    std::optional<std::vector<double>> FFT_Functional(std::vector<std::complex<double>> &a);

    std::optional<std::vector<double>> IFFT_Functional(std::vector<std::complex<double>> &a);
}

#endif //SCHRODINGER_1D_FFT_H
