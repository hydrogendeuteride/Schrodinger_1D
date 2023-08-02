#ifndef SCHRODINGER_1D_GRID_H
#define SCHRODINGER_1D_GRID_H

#include <vector>
#include <memory>
#include "shader.h"

class Grid
{
public:
    Grid();

    void GridGeneration(double Range_Min, double Range_Max, int Grid_Num);

    void Draw();

    void Setup(const std::shared_ptr<Shader> &shader);

    ~Grid();

private:
    std::vector<float> GridVertex;

    unsigned int VAO, VBO;

    std::shared_ptr<Shader> GridShader;
};

#endif //SCHRODINGER_1D_GRID_H
