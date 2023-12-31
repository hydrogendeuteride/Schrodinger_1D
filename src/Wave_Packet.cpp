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
    /*
     * wave packet time evolution scheme:
     * 1. propagate wave package half step forward in position space \psi * e^{-i * V * dt / 2.0}
     * 2. FFT(wave packet)
     * 3. propagate wave packet full step forward in momentum space \psi * e^{-i * T * dt}
     *      in momentum space, laplacian is -k^2(k = wave number)
     * 4. IFFT(wave packet)
     * 5. propagate wave package half step forward in position space \psi * e^{-i * V * dt / 2.0}
     *
     * \hbar and m is normalized to 1.0
     */
    std::vector<std::complex<double>> WaveFunction;
    for (int i = 0; i < Packet.size(); ++i)
    {
        WaveFunction.emplace_back(Packet[i].real(), Packet[i].imag());
    }

    for (int i = 0; i < WaveFunction.size(); ++i)
    {
        WaveFunction[i] *= std::exp(-I * Potential[i] * dt / 2.0);
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
        WaveFunction[i] *= std::exp(-I * Potential[i] * dt / 2.0);
    }

    for (int i = 0; i < WaveFunction.size(); ++i)
    {
        Packet[i] = WaveFunction[i];
    }

    normalize();
}

void Wave_Packet::TimePropagate(double dt, const Eigen::MatrixXd &Hamiltonian)
{
    /*
     * \psi * e^{-iHdt}
     */
    Eigen::MatrixXcd H_complex = -std::complex<double>(0, 1) * Hamiltonian * dt;
    Eigen::MatrixXcd U = H_complex.exp();

    Packet = (U * Packet).real();

    normalize();
}

std::vector<Eigen::Vector2d> Wave_Packet::GetDrawingData(int div)
{
    std::vector<double> x = Potential::XaxisGenerator(Grid_Num, Range_Min, Range_Max);

    std::vector<double> y;
    for (auto i: Packet)
    {
        y.push_back(i.real());
    }

    std::vector<Eigen::Vector2d> ret = SplinePoints(y.size(), div, x, y);

    return ret;
}

void Wave_Packet::normalize()
{
    double norm = 0.0;

    for (int i = 0; i < Packet.size(); ++i)
    {
        norm += std::abs(Packet[i]) * std::abs(Packet[i]);
    }
    norm = std::sqrt(norm);

    if (norm > 0.0)
    {
        Packet /= norm;
    }
}
