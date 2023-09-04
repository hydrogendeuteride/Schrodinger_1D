#ifndef SCHRODINGER_1D_WAVE_PACKET_H
#define SCHRODINGER_1D_WAVE_PACKET_H

#include <eigen3/Eigen/Dense>
#include "FFT.h"
#include "Potentials.h"
#include "spline.h"
#include <eigen3/unsupported/Eigen/MatrixFunctions>
#include <numeric>

const std::complex<double> I(0.0, 1.0);

class Wave_Packet
{
public:
    Wave_Packet();

    void PacketGeneration(int Grid_Num, double Range_Min, double Range_Max, double mu, double sigma, double k);//generate wave packet

    void TimePropagate(double dt, const std::vector<double>& Potential);//O(n log n) version, Split-step method

    void TimePropagate(double dt, const Eigen::MatrixXd &Hamiltonian);//O(n^2) version

    std::vector<Eigen::Vector2d> GetDrawingData(int div);//for drawing graph(use graph.h)
private:
    int Grid_Num;

    double Range_Max, Range_Min;

    Eigen::VectorXcd Packet;

    std::vector<double> c;

    double gaussian(double x, double mu, double sigma)
    {
        return (1.0 / (sigma * sqrt(2.0 * M_PI))) * exp(-0.5 * pow((x - mu) / sigma, 2));
    }

    double wavePacket(double x, double mu, double sigma, double k)
    {
        return gaussian(x, mu, sigma) * cos(k * x);
    }

    void normalize();//normalize wave packet
};

#endif //SCHRODINGER_1D_WAVE_PACKET_H
