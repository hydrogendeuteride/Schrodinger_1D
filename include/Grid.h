#ifndef SCHRODINGER_1D_GRID_H
#define SCHRODINGER_1D_GRID_H

#include <vector>
#include <memory>
#include "shader.h"
#include "Color.h"

class Grid
{
public:
    Grid();

    void Draw(Color color);//draw grid with color

    void Setup(const std::shared_ptr<Shader> &shader, double Range_Min, double Range_Max, int Grid_Num);//setup grid, shader etc

private:
    std::vector<float> GridGeneration(double Range_Min, double Range_Max, int Grid_Num);//generate grid with grid number

    unsigned int VAO, VBO;

    std::shared_ptr<Shader> GridShader;

    int Vertex_Num;
};

#endif //SCHRODINGER_1D_GRID_H
