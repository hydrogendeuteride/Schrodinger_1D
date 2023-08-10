#ifndef SCHRODINGER_1D_POTENTIALS_H
#define SCHRODINGER_1D_POTENTIALS_H

#include <vector>

namespace Potential
{
    std::vector<double> HarmonicOscillatorPotential(int Num_Grid, double Coeff, std::vector<double> &x)
    {
        std::vector<double> tmp;
        tmp.reserve(x.size());
        for (const auto t: x)
        {
            tmp.push_back(Coeff * t * t);
        }

        return tmp;
    }

    std::vector<double> PotentialGenerator(int Grid_Num, double Range_Min, double Range_Max)
    {
        double dx = (Range_Max - Range_Min) / static_cast<double>(Grid_Num);
        std::vector<double> x(Grid_Num);
        std::generate(x.begin(), x.end(), [Range_Min, dx]()mutable {
            auto current = Range_Min;
            Range_Min += dx;
            return current;
        });

        return x;
    }
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
