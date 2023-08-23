#include "Render.h"
#include <numeric>
#include <cstdlib>

Render::Render(int width, int height) : SCR_WIDTH(width), SCR_HEIGHT(height)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Schrodinger 1D", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *w, int width, int height) {
        auto *win = static_cast<Render *>(glfwGetWindowUserPointer(w));
        win->framebuffer_size_callback(width, height);
    });

    glfwSetScrollCallback(window, [](GLFWwindow *w, double xoffset, double yoffset) {
        auto *win = static_cast<Render *>(glfwGetWindowUserPointer(w));
        win->scroll_callback(xoffset, yoffset);
    });

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    shader.setShader("../shader/shader.vert", "../shader/shader.frag");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Render::framebuffer_size_callback(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Render::scroll_callback(double xoffset, double yoffset)
{
    fov -= (float) yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 60.0f)
        fov = 60.0f;
}

void Render::processinput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraspeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        CamPos += cameraspeed * CamUp;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        CamPos -= cameraspeed * CamUp;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        CamPos -= glm::normalize(glm::cross(CamFront, CamUp)) * cameraspeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        CamPos += glm::normalize(glm::cross(CamFront, CamUp)) * cameraspeed;
}

void Render::Setup(int Grid_Num, double Range_Min, double Range_Max, std::vector<double> &Potential)
{
    FDM_Solver solver(Grid_Num, Range_Min, Range_Max);

    solution = solver.Get_Solution(true, Potential);

    x = Potential::XaxisGenerator(Grid_Num, Range_Min, Range_Max);
    std::vector<double> y(solution[0].second.data(), solution[0].second.data() + solution[0].second.size());

    //if y.size ! = x.size error throw

    std::vector<Eigen::Vector2d> GraphPlot = SplinePoints(Grid_Num, 10, x, y);
    graph.setup(GraphPlot, std::make_shared<Shader>(shader));

    std::vector<Eigen::Vector2d> PotentialPlot = SplinePoints(Grid_Num, 10, x, Potential);
    potential.setup(PotentialPlot, std::make_shared<Shader>(shader));

    grid.Setup(std::make_shared<Shader>(shader), Range_Min * 2, Range_Max * 2, static_cast<int>(Range_Max - Range_Min) * 2);
}

void Render::ChangeGraph(int eigenvalue)
{
    std::vector<double> y(solution[eigenvalue].second.data(),
                          solution[eigenvalue].second.data() + solution[eigenvalue].second.size());

    std::vector<Eigen::Vector2d> GraphPlot = SplinePoints(y.size(), 10, x, y);

    graph.Update(GraphPlot);
}

void Render::Draw(Color GraphColor, Color GridColor)
{

    double tmp = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        processinput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                100.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(CamPos, CamPos + CamFront, CamUp);
        shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);

        graph.draw(Blue);
        potential.draw(White);

        grid.Draw(White);

        ImGui::SetNextWindowPos(ImVec2(100, 100));
        ImGui::SetNextWindowSize(ImVec2(400, 300));

        ImGuiIO &io = ImGui::GetIO();
        io.FontGlobalScale = 1.5f;

        ImGui::Begin("window", nullptr,
                     ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        if (ImGui::BeginCombo("Eigenvalues", std::to_string(solution[selecteditem].first / solution[0].first).c_str()))
        {
            int previousSelectedItem = selecteditem;

            for (int i = 0; i < solution.size(); i++)
            {
                bool isSelected = (i == selecteditem);
                if (ImGui::Selectable(std::to_string(solution[i].first / solution[0].first).c_str(), isSelected))
                {
                    selecteditem = i;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            if (previousSelectedItem != selecteditem)
            {
                tmp = 0.0;
                ChangeGraph(selecteditem);
            }

            ImGui::EndCombo();
        }

        tmp += 0.01;

        ImGui::Checkbox("time dependant", &check);

        if (check)
        {
            graph.TimePropagate(solution[selecteditem].first / solution[0].first, tmp);
        }
        else
        {
            tmp = 0.0;
            graph.TimePropagate(0, 0);
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}