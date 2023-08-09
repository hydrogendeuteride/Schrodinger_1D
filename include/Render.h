#ifndef SCHRODINGER_1D_RENDER_H
#define SCHRODINGER_1D_RENDER_H

#include "Graph.h"
#include "spline.h"
#include "FDM_Solver.h"
#include "Potentials.h"
#include "shader.h"
#include "Grid.h"

class Render
{
public:
    Render(int width, int height);

    void Setup(int Grid_Num, double Range_Min, double Range_Max, std::vector<double> &Potential);

    void Draw(Color GraphColor, Color GridColor);

private:
    GLFWwindow *window;

    static void framebuffer_size_callback(int width, int height);

    const int SCR_WIDTH, SCR_HEIGHT;

    Shader shader = Shader("shader/graph.vert", "shader/graph.frag");

    Graph graph;
    Graph potential;

    Grid grid;
};

#endif //SCHRODINGER_1D_RENDER_H
