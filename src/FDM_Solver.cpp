#include "FDM_Solver.h"

FDM_Solver::FDM_Solver(int Num_Grid, double range_min, double range_max)
        : num_grid(Num_Grid), range_min(range_min), range_max(range_max)
{
    dx = (range_max - range_min) / static_cast<double>(num_grid);
}

void FDM_Solver::Solve(const std::vector<double> &Potentials)
{
    Eigen::MatrixXd Hamiltonian(num_grid - 2, num_grid - 2);

    Hamiltonian.setZero();

    for (int i = 0; i < num_grid - 2; ++i)
    {
        Hamiltonian(i, i) = 1.0 / (dx * dx) + Potentials[i + 1];

        if (i != num_grid - 3)
        {
            Hamiltonian(i, i + 1) = -1.0 / (2.0 * dx * dx);
            Hamiltonian(i + 1, i) = -1.0 / (2.0 * dx * dx);
        }
    }

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> s(Hamiltonian);

    Eigen::MatrixXd eigenvectors = s.eigenvectors();
    const Eigen::VectorXd& eigenvalues = s.eigenvalues();

    // Assuming EigenVector is a matrix to store the full eigenvectors with boundary values
    EigenVector = Eigen::MatrixXd(num_grid, eigenvectors.cols());

    for (int col = 0; col < eigenvectors.cols(); ++col)
    {
        EigenVector(0, col) = 0.0;
        for (int i = 1; i < num_grid - 1; ++i)
        {
            EigenVector(i, col) = eigenvectors(i-1, col);
        }
        EigenVector(num_grid - 1, col) = 0.0;
    }

    EigenValue = eigenvalues;
}

std::vector<std::pair<double, Eigen::VectorXd>> FDM_Solver::Get_Solution
(bool sorted, const std::vector<double> &Potentials)
{
    Solve(Potentials);

    std::vector<std::pair<double, Eigen::VectorXd >> ret;

    for (int i = 0; i < num_grid - 2; ++i)
    {
        ret.emplace_back(EigenValue(i), EigenVector.col(i).normalized());
    }

    if (sorted)
    {
        auto comp = []
                (const std::pair<double, Eigen::VectorXd >&a,
                const std::pair<double, Eigen::VectorXd >&b)
                        {
                            return a.first < b.first;
                        };

        std::sort(std::begin(ret), std::end(ret), comp);

        return ret;
    }

    return ret;
}