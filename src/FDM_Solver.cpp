#include "FDM_Solver.h"

FDM_Solver::FDM_Solver(int Num_Grid, double range_min, double range_max)
        : num_grid(Num_Grid), range_min(range_min), range_max(range_max)
{
    dx = (range_max - range_min) / num_grid;
}

std::vector<double> FDM_Solver::Solve(const std::vector<double> &Potentials)
{
    Eigen::MatrixXd Hamiltonian(num_grid, num_grid);

    for (int i = 0; i < num_grid; ++i)
    {
        Hamiltonian(i, i) = 1.0 / (dx * dx) + Potentials[i];
    }

    for (int i = 0; i < num_grid - 1; ++i)
    {
        Hamiltonian(i, i + 1) = 1.0 / (2 * dx * dx);
        Hamiltonian(i + 1, i) = 1.0 / (dx * dx);
    }

    Eigen::EigenSolver<Eigen::MatrixXd> s(Hamiltonian);

    EigenVector = s.eigenvectors();
    EigenValue = s.eigenvalues();
}

std::vector<std::pair<std::complex<double>, Eigen::VectorXd>> FDM_Solver::Get_Solution(bool sorted)
{
    std::vector<std::pair<std::complex<double>, Eigen::VectorXcd >> ret;

    if (sorted)
    {
        for (int i = 0; i < num_grid; ++i)
        {
            ret.emplace_back(EigenValue(i), EigenVector.col(i));
        }
    }
}