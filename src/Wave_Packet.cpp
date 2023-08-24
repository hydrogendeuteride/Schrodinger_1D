#include "Wave_Packet.h"

Wave_Packet::Wave_Packet()
{
}

void Wave_Packet::PacketGeneration(int Grid_Num, double Range_Min, double Range_Max, double mu, double sigma, double k)
{
    this->Grid_Num = Grid_Num;
    this->Range_Min = Range_Min;
    this->Range_Max = Range_Max;

    double dx = (Range_Max - Range_Min) / static_cast<double>(Grid_Num);
    std::vector<double> x(Grid_Num);
    std::generate(x.begin(), x.end(), [Range_Min, dx]()mutable {
        auto current = Range_Min;
        Range_Min += dx;
        return current;
    });

    std::vector<double> y;
    for (const auto &t: x)
    {
        y.push_back(wavePacket(t, mu, sigma, k));
    }

    Packet.resize(Grid_Num);
    for (int i = 0; i < Grid_Num; ++i)
    {
        Packet[i] = y[i];
    }

    c.reserve(Grid_Num);
}

void Wave_Packet::TimePropagate(double dt, const std::vector<std::pair<double, Eigen::VectorXd>> &EigenVectors)
{
    for (int i = 0; i < 32; ++i)
    {
        c[i] = Packet.dot(EigenVectors[i].second);
    }

    Eigen::VectorXd wave;
    wave.resize(Grid_Num);
    for (int i = 0; i < 32; ++i)
    {
        wave += c[i] * EigenVectors[i].second;
    }

    std::vector<std::complex<double>> wave_complex(Grid_Num);
    for (int i = 0; i < Grid_Num; ++i)
    {
        wave_complex[i] = std::complex<double>(wave[i], 0.0);
    }

    FFT::FFT(wave_complex);

    double dk = 2 * PI / Grid_Num;
    for (int i = 0; i < Grid_Num; ++i)
    {
        double k = i < Grid_Num / 2 ? i * dk : (i - Grid_Num) * dk;
        double energy = 0.5 * k * k;
        std::complex<double> phase(0, -energy * dt);
        wave_complex[i] *= std::exp(phase);
    }

    FFT::IFFT(wave_complex);

    for (int i = 0; i < wave_complex.size(); ++i)
    {
        Packet(i) = wave_complex[i].real();
    }

    normalize();
}

std::vector<Eigen::Vector2d> Wave_Packet::GetDrawingData(int div)
{
    std::vector<double> x = Potential::XaxisGenerator(Grid_Num, Range_Min, Range_Max);

    std::vector<double> y(Packet.data(), Packet.data() + Packet.size());

    std::vector<Eigen::Vector2d> ret = SplinePoints(y.size(), div, x, y);

    return ret;
}

void Wave_Packet::normalize()
{
    double norm = std::sqrt(Packet.squaredNorm());
    if (norm > 0.0)
    {
        Packet /= norm;
    }
}
