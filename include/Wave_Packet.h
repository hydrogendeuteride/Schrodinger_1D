#ifndef SCHRODINGER_1D_WAVE_PACKET_H
#define SCHRODINGER_1D_WAVE_PACKET_H

#include <eigen3/Eigen/Dense>
#include "FFT.h"

class Wave_Packet
{
public:
    Wave_Packet();

    void PacketGeneration(int Grid_Num, double Range_Max, double Range_Min, double mu, double sigma, double k);

    void TimePropagate(double dt, const std::vector<std::pair<double, Eigen::VectorXd>>& EigenVectors);

private:
    int Grid_Num;

    double Range_Max, Range_Min;

    Eigen::VectorXd Packet;

    std::vector<double> c;

    double gaussian(double x, double mu, double sigma)
    {
        return (1.0 / (sigma * sqrt(2.0 * M_PI))) * exp(-0.5 * pow((x - mu) / sigma, 2));
    }

    double wavePacket(double x, double mu, double sigma, double k)
    {
        return gaussian(x, mu, sigma) * cos(k * x);
    }
};

#endif //SCHRODINGER_1D_WAVE_PACKET_H
