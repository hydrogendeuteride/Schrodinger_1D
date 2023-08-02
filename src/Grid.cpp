#include "Grid.h"

Grid::Grid()
{

}

void Grid::GridGeneration(double Range_Min, double Range_Max, int Grid_Num)
{
    double dx = (Range_Max - Range_Min) / static_cast<double>(Grid_Num);

    for (int i = 0; i <= Grid_Num; ++i)
    {
        for (int j = 0; j <= Grid_Num; ++j)
        {
            float x = static_cast<float>(i) * static_cast<float>(dx);
            float y = static_cast<float>(i) * static_cast<float>(dx);

            GridVertex.push_back(x);
            GridVertex.push_back(y);
            GridVertex.push_back(0.0f);
        }
    }
}