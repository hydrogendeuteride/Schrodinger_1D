#ifndef SCHRODINGER_1D_RENDER_H
#define SCHRODINGER_1D_RENDER_H

#include "Graph.h"
#include "spline.h"
#include "FDM_Solver.h"
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

    void framebuffer_size_callback(int width, int height);

    void scroll_callback(double xoffset, double yoffset);

    void processinput(GLFWwindow *window);

    const int SCR_WIDTH, SCR_HEIGHT;

    glm::vec3 CamPos = glm::vec3(0.0f, 0.0f, 2.0f);
    glm::vec3 CamFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 CamUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float fov = 60.0;

    Shader shader = Shader("shader/graph.vert", "shader/graph.frag");

    Graph graph;
    Graph potential;

    Grid grid;
};

#endif //SCHRODINGER_1D_RENDER_H
