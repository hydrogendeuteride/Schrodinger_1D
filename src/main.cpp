#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include <vector>
#include "Render.h"
#include "Potentials.h"

constexpr unsigned int SCR_WIDTH = 1920, SCR_HEIGHT = 1200;

int main()
{
    Render render(SCR_WIDTH, SCR_HEIGHT);

    std::vector<double> x = Potential::XaxisGenerator(1024, -6.0, 6.0);
    std::vector<double> p = Potential::HarmonicOscillatorPotential(1024, 1.0, x);
    render.Setup(1024, -6.0, 6.0, p);

    render.Draw(Blue, White);

    return 0;
}
