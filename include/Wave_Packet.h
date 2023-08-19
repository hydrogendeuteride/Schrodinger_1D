#ifndef SCHRODINGER_1D_WAVE_PACKET_H
#define SCHRODINGER_1D_WAVE_PACKET_H

#include <eigen3/Eigen/Dense>
#include "FFT.h"

class Wave_Packet
{
public:
    Wave_Packet();

    std::vector<std::pair<double, Eigen::VectorXd>> a;
};

#endif //SCHRODINGER_1D_WAVE_PACKET_H
