#include "Grid.h"

Grid::Grid()
{

}

std::vector<float> Grid::GridGeneration(double Range_Min, double Range_Max, int Grid_Num)
{
    std::vector<float> vertex;

    double dx = (Range_Max - Range_Min) / static_cast<double>(Grid_Num);

    for (int i = 0; i <= Grid_Num; ++i)
    {
        for (int j = 0; j <= Grid_Num; ++j)
        {
            float x = static_cast<float>(i) * static_cast<float>(dx) - static_cast<float>(Range_Min);
            float y = static_cast<float>(j) * static_cast<float>(dx) - static_cast<float>(Range_Min);

            vertex.push_back(x);
            vertex.push_back(y);
            vertex.push_back(0.0f);
        }
    }

    return vertex;
}

void Grid::Setup(const std::shared_ptr<Shader> &shader, double Range_Min, double Range_Max, int Grid_Num)
{
    std::vector<float> vertex = GridGeneration(Range_Min, Range_Max, Grid_Num);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), static_cast<void *>(vertex.data()), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    GridShader = shader;

    Vertex_Num = (Grid_Num + 1) * (Grid_Num + 1);
}

void Grid::Draw(Color color)
{
    switch (color)
    {
        case Red:
            GridShader->setVec4("color", glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
            break;
        case Green:
            GridShader->setVec4("color", glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
            break;
        case Blue:
            GridShader->setVec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
            break;
        case White:
            GridShader->setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
            break;
        default:
            GridShader->setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
            break;
    }

    GridShader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, Vertex_Num);
}

Grid::~Grid()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}