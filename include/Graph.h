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
#include "Color.h"

class Graph
{
public:
    Graph(const std::shared_ptr<Shader> &shader);

    Graph()
    {};

    void setup(const std::vector<Eigen::Vector2d> &data, const std::shared_ptr<Shader> &shader);//setup shaders, meshes

    void Update(const std::vector<Eigen::Vector2d> &data);//update graph vertex

    void TimePropagate(double energy, double dt);//just for eigenstate time evolution

    void draw(Color color);//draw with color

private:
    unsigned int VAO, VBO;

    std::vector<Eigen::Vector2d> graph;

    std::shared_ptr<Shader> GraphShader;
    bool ShaderInitialized = false;
};

#endif //SCHRODINGER_1D_GRAPH_H
