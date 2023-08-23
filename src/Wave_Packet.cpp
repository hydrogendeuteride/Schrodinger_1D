#include "Wave_Packet.h"

Wave_Packet::Wave_Packet()
{
}

void Wave_Packet::PacketGeneration(int Grid_Num, double Range_Max, double Range_Min, double mu, double sigma, double k)
{
    double dx = (Range_Max - Range_Min) / static_cast<double>(Grid_Num);
    std::vector<double> x(Grid_Num);
    std::generate(x.begin(), x.end(), [Range_Min, dx]()mutable {
        auto current = Range_Min;
        Range_Min += dx;
        return current;
    });

    std::vector<double> y;
    for (const auto& t: x)
    {
        y.push_back(wavePacket(t, mu, sigma, k));
    }

    Packet = Eigen::Map<Eigen::VectorXd>(y.data(), y.size());

    c.reserve(Grid_Num);
}

void Wave_Packet::TimePropagate(double dt, const std::vector<std::pair<double, Eigen::VectorXd>>& EigenVectors)
{
    for (int i = 0; i < Grid_Num; ++i)
    {
        c[i] = Packet.dot(EigenVectors[i].second);
    }

    Eigen::VectorXd wave;
    for (int i = 0; i < Grid_Num; ++i)
    {
        wave += c[i] * EigenVectors[i].second;
    }

    std::vector<std::complex<double>> wave_complex(Grid_Num);
    for (int i = 0; i < Grid_Num; ++i)
    {
        wave_complex[i] = std::complex<double>(wave[i], 0.0);
    }

    FFT::FFT(wave_complex);
}