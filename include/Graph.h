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
#include <memory>

enum Color
{
    Red,
    Green,
    Blue
};

class Graph
{
public:
    Graph(const std::shared_ptr<Shader> &shader);

    Graph()
    {};

    ~Graph();

    void setup(const std::vector<Eigen::Vector2d> &data, const std::shared_ptr<Shader> &shader);

    void Update(const std::vector<Eigen::Vector2d> &data);

    void draw(Color color);

private:
    unsigned int VAO, VBO;

    std::vector<Eigen::Vector2d> graph;

    std::shared_ptr<Shader> GraphShader;
    bool ShaderInitialized = false;
};

#endif //SCHRODINGER_1D_GRAPH_H
