#ifndef SCHRODINGER_1D_GRAPH_H
#define SCHRODINGER_1D_GRAPH_H

#include <vector>
#include <eigen3/Eigen/Dense>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <string>

enum Color
{
    Red,
    Green,
    Blue
};

class Graph
{
public:
    Graph(const std::vector<Eigen::Vector2d> &data, const Shader &shader);

    void setup();

    void draw(Color color);

private:
    unsigned int VAO, VBO;

    std::vector<Eigen::Vector2d> graph;

    Shader shader;
};

#endif //SCHRODINGER_1D_GRAPH_H
