#include "FFT.h"

void FFT::FFT(std::vector<std::complex<double>> &a)
{
    int n = a.size();
    int log_n = 0;
    while ((1 << log_n) < n)
        log_n++;

    for (int i = 0; i < n; ++i)
    {
        int j = 0;
        for (int k = 0; k < log_n; ++k)
        {
            j = (j << 1) | ((i >> k) & 1);
        }
        if (i < j)
            std::swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1)
    {
        double ang = 2 * PI / len;
        std::complex<double> wlen(std::cos(ang), std::sin(ang));
        for (int i = 0; i < n; i += len)
        {
            std::complex<double> w(1);
            for (int j = 0; j < len / 2; j++)
            {
                std::complex<double> u = a[i + j];
                std::complex<double> v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
}

void FFT::IFFT(std::vector<std::complex<double>> &a)
{
    int n = a.size();

    for (int i = 0; i < n; ++i)
    {
        a[i].imag(-a[i].imag());
    }

    FFT(a);

    for (int i = 0; i < n; ++i)
    {
        a[i] /= n;
        a[i].imag(-a[i].imag());
    }
}

std::optional<std::vector<double>> FFT::FFT_Functional(std::vector<std::complex<double>> &a)
{
    auto isPowerOfTwo = [](int n) ->bool{
        if (n == 0) return false;
        return (n & (n - 1)) == 0;
    };

    if (isPowerOfTwo(a.size()))
    {
        return std::nullopt;
    }

    std::vector<double> ret;
    FFT(a);

    for (const auto &x: a)
    {
        ret.emplace_back(x.real());
    }

    return ret;
}

std::optional<std::vector<double>> FFT::IFFT_Functional(std::vector<std::complex<double>> &a)
{
    auto isPowerOfTwo = [](int n) ->bool{
        if (n == 0) return false;
        return (n & (n - 1)) == 0;
    };

    if (isPowerOfTwo(a.size()))
    {
        return std::nullopt;
    }

    std::vector<double> ret;
    IFFT(a);

    for (const auto &x: a)
    {
        ret.emplace_back(x.real());
    }

    return ret;
}