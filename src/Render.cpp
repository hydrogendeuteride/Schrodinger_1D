#include "Render.h"
#include <numeric>

Render::Render()
{}

void Render::Setup(int Grid_Num, double Range_Min, double Range_Max, std::vector<double> &Potential)
{
    FDM_Solver solver(Grid_Num, Range_Min, Range_Max);

    const auto sol = solver.Get_Solution(true, Potential);

    double dx = (Range_Max - Range_Min) / static_cast<double>(Grid_Num);
    std::vector<double> x(Grid_Num);
    std::generate(x.begin(), x.end(), [Range_Min, dx]()mutable {
        auto current = Range_Min;
        Range_Min += dx;
        return current;
    });

    std::vector<double> y(sol[0].second.data(), sol[0].second.data() + sol[0].second.size());

    //if y.size ! = x.size error throw

    GraphPlot = SplinePoints(Grid_Num, 10, x, y);
    graph.setup(GraphPlot, std::make_shared<Shader>(shader));
}

void Render::Draw(Color GraphColor, Color GridColor)
{
    graph.draw(Blue);
}