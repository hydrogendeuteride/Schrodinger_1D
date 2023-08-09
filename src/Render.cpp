#include "Render.h"
#include <numeric>

Render::Render(int width, int height) : SCR_WIDTH(width), SCR_HEIGHT(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Schrodinger 1D", glfwGetPrimaryMonitor(), nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow * w, int width, int height)
    {
        auto *win = static_cast<Render*>(glfwGetWindowUserPointer(w));
        win->framebuffer_size_callback(width, height);
    });

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
}

void Render::framebuffer_size_callback(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Render::Setup(int Grid_Num, double Range_Min, double Range_Max, std::vector<double> &Potential)
{
    FDM_Solver solver(Grid_Num, Range_Min, Range_Max);

    const auto sol = solver.Get_Solution(true, Potential);

    double dx = (Range_Max - Range_Min) / static_cast<double>(Grid_Num);
    std::vector<double> x(Grid_Num);
    std::generate(x.begin(), x.end(), [Range_Min, dx]()mutable {
        auto current = Range_Min;
        Range_Min += dx;
        return current;
    });

    std::vector<double> y(sol[0].second.data(), sol[0].second.data() + sol[0].second.size());

    //if y.size ! = x.size error throw

    std::vector<Eigen::Vector2d> GraphPlot = SplinePoints(Grid_Num, 10, x, y);
    graph.setup(GraphPlot, std::make_shared<Shader>(shader));

    std::vector<Eigen::Vector2d> PotentialPlot = SplinePoints(Grid_Num, 10, x, Potential);
    potential.setup(PotentialPlot, std::make_shared<Shader>(shader));

    grid.Setup(std::make_shared<Shader>(shader), Range_Min, Range_Max, Grid_Num);
}

void Render::Draw(Color GraphColor, Color GridColor)
{

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        graph.draw(Blue);
        potential.draw(White);

        grid.Draw(White);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}