add_executable(Tests Schrodinger_Solver_Test.cpp ../src/FDM_Solver.cpp ../src/Spline.cpp Spline_Test.cpp)
target_link_libraries(Tests gtest_main )

include(GoogleTest)
gtest_discover_tests(Tests)