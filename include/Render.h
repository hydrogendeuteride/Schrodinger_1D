#ifndef SCHRODINGER_1D_RENDER_H
#define SCHRODINGER_1D_RENDER_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Graph.h"
#include "spline.h"
#include "FDM_Solver.h"
#include "shader.h"
#include "Grid.h"
#include "Potentials.h"
#include "Wave_Packet.h"

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

    void ChangeGraph(int eigenvalue);

    const int SCR_WIDTH, SCR_HEIGHT;

    std::vector<std::pair<double, Eigen::VectorXd>> solution;

    int selecteditem = 1;
    bool check = 0;

    std::vector<double> x;

    glm::vec3 CamPos = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 CamFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 CamUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float fov = 60.0;

    Shader shader;

    Graph graph;
    Graph potential;
    Graph packet;

    Wave_Packet wavePacket;

    Grid grid;
};

#endif //SCHRODINGER_1D_RENDER_H
