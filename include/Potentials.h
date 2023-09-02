#ifndef SCHRODINGER_1D_POTENTIALS_H
#define SCHRODINGER_1D_POTENTIALS_H

#include <vector>

namespace Potential
{
    std::vector<double> HarmonicOscillatorPotential(int Num_Grid, double Coeff, std::vector<double> &x);

    std::vector<double> InfiniteSquareWell(int Num_Grid, double start, double end, double MinRange);

    std::vector<double> FiniteSquareWell(int Num_Grid, double start, double end, double wellDepth);

    std::vector<double> XaxisGenerator(int Grid_Num, double Range_Min, double Range_Max);
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
