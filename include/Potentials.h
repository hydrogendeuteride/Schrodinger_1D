#ifndef SCHRODINGER_1D_POTENTIALS_H
#define SCHRODINGER_1D_POTENTIALS_H

#include <vector>

namespace Potential
{
    std::vector<double> HarmonicOscillatorPotential(int Num_Grid, double Coeff, std::vector<double> &x);//Coeff * x * x

    std::vector<double> InfiniteSquareWell(int Num_Grid, double start, double end, double MinRange);

    std::vector<double> FiniteSquareWell(int Num_Grid, double start, double end, double wellDepth, double MinRange);

    std::vector<double> XaxisGenerator(int Grid_Num, double Range_Min, double Range_Max);//equal grid generator in range [R_min, R_max]

    std::vector<double> DiracDelta(int Num_Grid, double position, double MinRange);
}

enum PotentialType
{
    HarmonicOscillator,
    InfiniteWell,
    FiniteWell,
    DiracDelta,
    FreeSpace
};

#endif //SCHRODINGER_1D_POTENTIALS_H
