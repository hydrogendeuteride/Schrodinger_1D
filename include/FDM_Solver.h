#ifndef SCHRODINGER_1D_FDM_SOLVER_H
#define SCHRODINGER_1D_FDM_SOLVER_H

#include <eigen3/Eigen/Dense>
#include <vector>
#include <algorithm>

class FDM_Solver//eigenstate solver for 1d schrodinger equation
{
public:
    FDM_Solver(int Num_Grid, double range_min, double range_max);

    std::vector<std::pair<double, Eigen::VectorXd>> Get_Solution
    (bool sorted, const std::vector<double> &Potentials);//get sorted <eigenvalus, eigenvector> pair vector

    Eigen::MatrixXd Hamiltonian;//just for boilerplate code

private:
    int num_grid;
    std::vector<double> Potentials;

    double range_min;
    double range_max;

    double dx;

    void Solve(const std::vector<double>& Potentials);//solve eigenstate problem

    Eigen::MatrixXd EigenVector;
    Eigen::VectorXd EigenValue;
};

#endif //SCHRODINGER_1D_FDM_SOLVER_H
