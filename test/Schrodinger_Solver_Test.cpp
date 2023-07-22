#include <gtest/gtest.h>
#include "FDM_Solver.h"

#include <limits>

TEST(FDM_SolverTest, TestSolve){
    FDM_Solver solver(100, -6.0, 6.0);

    std::vector<double> Potential(100, 0);

    Potential[0] = Potential[99] = 1000;

    auto sol = solver.Get_Solution(true, Potential);

    for (int i = 0; i < 100; ++i)
    {
        std::cout << sol[i].first / sol[0].first << "\n";
    }

    EXPECT_NEAR(sol[1].first, 1.0, 0.1);
}