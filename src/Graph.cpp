#include "Graph.h"

Graph::Graph(const std::shared_ptr<Shader> &shader)
        : GraphShader(shader)
{
    ShaderInitialized = true;
}

void Graph::setup(const std::vector<Eigen::Vector2d> &data, const std::shared_ptr<Shader> &shader)
{
    std::vector<float> vertex;

    graph = data;

    for (const auto &x: graph)
    {
        vertex.push_back(static_cast<float>(x(0)));
        vertex.push_back(static_cast<float>(x(1)));
        vertex.push_back(0.0f);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), static_cast<void *>(vertex.data()), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    if (!ShaderInitialized)
        GraphShader = shader;
}

void Graph::Update(const std::vector<Eigen::Vector2d> &data)
{
    graph = data;
    std::vector<float> vertex;

    for (const auto &x: graph)
    {
        vertex.push_back(static_cast<float>(x(0)));
        vertex.push_back(static_cast<float>(x(1)));
        vertex.push_back(0.0f);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), static_cast<void *>(vertex.data()), GL_DYNAMIC_DRAW);
}

void Graph::draw(Color color)
{
    switch (color)
    {
        case Red:
            GraphShader->setVec4("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            break;
        case Green:
            GraphShader->setVec4("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            break;
        case Blue:
            GraphShader->setVec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
            break;
        case White:
            GraphShader->setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        default:
            GraphShader->setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            break;
    }

    GraphShader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, graph.size());
}