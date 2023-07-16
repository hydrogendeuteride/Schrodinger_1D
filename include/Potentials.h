#ifndef SCHRODINGER_1D_POTENTIALS_H
#define SCHRODINGER_1D_POTENTIALS_H

#include <vector>

namespace Potential
{
    std::vector<double> HarmonicOscillatorPotential(int Num_Grid, double Coeff, std::vector<double>& x)
    {
        std::vector<double> tmp;
        tmp.reserve(x.size());
        for (const auto t: x)
        {
            tmp.push_back(Coeff * t * t);
        }

        return tmp;
    }
}

#endif //SCHRODINGER_1D_POTENTIALS_H
