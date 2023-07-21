#ifndef SCHRODINGER_1D_FDM_SOLVER_H
#define SCHRODINGER_1D_FDM_SOLVER_H

#include <eigen3/Eigen/Dense>
#include <vector>
#include "Potentials.h"

class FDM_Solver
{
public:
    FDM_Solver(int Num_Grid, double range_min, double range_max);

    std::vector<std::pair<std::complex<double>, Eigen::VectorXd>> Get_Solution(bool sorted);

private:
    int num_grid;
    std::vector<double> Potentials;

    double range_min;
    double range_max;

    double dx;

    std::vector<double> Solve(const std::vector<double>& Potentials);

    Eigen::MatrixXcd EigenVector;
    Eigen::VectorXcd EigenValue;
};

#endif //SCHRODINGER_1D_FDM_SOLVER_H
