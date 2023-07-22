add_executable(Solver_Test Schrodinger_Solver_Test.cpp ../src/FDM_Solver.cpp)
target_link_libraries(Solver_Test gtest_main )

include(GoogleTest)
gtest_discover_tests(Solver_Test)