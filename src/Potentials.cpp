#include "Potentials.h"

constexpr double INFINITE_POTENTIAL = 10.0;

std::vector<double> Potential::HarmonicOscillatorPotential(int Num_Grid, double Coeff, std::vector<double> &x)
{
    std::vector<double> tmp;
    tmp.reserve(x.size());
    for (const auto t: x)
    {
        tmp.push_back(Coeff * t * t);
    }

    return tmp;
}

std::vector<double> Potential::InfiniteSquareWell(int Num_Grid, double start, double end, double MinRange)
{
    std::vector<double> potential(Num_Grid, INFINITE_POTENTIAL);

    int startIndex = static_cast<int>((start - MinRange) / 12.0 * Num_Grid);
    int endIndex = static_cast<int>((end-MinRange) / 12.0 * Num_Grid);

    for (int i = startIndex; i < endIndex; i++) {
        potential[i] = 0.0;
    }

    return potential;
}

std::vector<double> Potential::FiniteSquareWell(int Num_Grid, double start, double end, double wellDepth)
{
    std::vector<double> potential(Num_Grid, 0.0);

    int startIndex = static_cast<int>(start * Num_Grid);
    int endIndex = static_cast<int>(end * Num_Grid);

    for (int i = startIndex; i < endIndex; i++) {
        potential[i] = wellDepth;
    }

    return potential;
}

std::vector<double> Potential::XaxisGenerator(int Grid_Num, double Range_Min, double Range_Max)
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