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

    normalize();
}

void Wave_Packet::TimePropagate(double dt, const std::vector<double> &Potential)
{
    std::vector<std::complex<double>> WaveFunction;
    for (int i = 0; i < Packet.size(); ++i)
    {
        WaveFunction.emplace_back(Packet[i], 0);
    }

    for (int i = 0; i < WaveFunction.size(); ++i)
    {
        WaveFunction[i] *= std::exp(I * Potential[i] * dt / 2.0);
    }

    FFT::FFT(WaveFunction);

    double dk = 2 * PI / Grid_Num;
    for (int i = 0; i < Grid_Num; ++i)
    {
        double k = i < Grid_Num / 2 ? i * dk : (i - Grid_Num) * dk;
        double energy = 0.5 * k * k;
        std::complex<double> phase(0, -energy * dt);
        WaveFunction[i] *= std::exp(phase);
    }

    FFT::IFFT(WaveFunction);

    for (int i = 0; i < WaveFunction.size(); ++i)
    {
        WaveFunction[i] *= std::exp(I * Potential[i] * dt / 2.0);
    }

    for (int i = 0; i < WaveFunction.size(); ++i)
    {
        Packet[i] = WaveFunction[i].real();
    }

    normalize();
}

void Wave_Packet::TimePropagate(double dt, const Eigen::MatrixXd &Hamiltonian)
{
    Eigen::MatrixXcd H_complex = -std::complex<double>(0, 1) * Hamiltonian * dt;
    Eigen::MatrixXcd U = H_complex.exp();

    Packet = (U *  Packet).real();

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
    double t = Packet.array().square().sum() * (Range_Max - Range_Min) / static_cast<double>(Grid_Num);

    double norm = std::sqrt(t);
    Packet /= norm;
}
