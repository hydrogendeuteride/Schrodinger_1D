#include "FDM_Solver.h"

FDM_Solver::FDM_Solver(int Num_Grid, double range_min, double range_max)
:num_grid(Num_Grid), range_min(range_min), range_max(range_max)
{}

std::vector<double> FDM_Solver::solve(std::vector<double>& Potentials)
{
    Eigen::MatrixXd Hamiltonian;

    Hamiltonian.setIdentity(num_grid, num_grid);
}