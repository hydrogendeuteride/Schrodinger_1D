#include "Render.h"
#include <numeric>
#include <cstdlib>
#include <future>
#include <chrono>

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
        Render::framebuffer_size_callback(width, height);
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

void Render::processinput(GLFWwindow *pWwindow)
{
    if (glfwGetKey(pWwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(pWwindow, true);

    float cameraspeed = 0.05f;
    if (glfwGetKey(pWwindow, GLFW_KEY_W) == GLFW_PRESS)
        CamPos += cameraspeed * CamUp;
    if (glfwGetKey(pWwindow, GLFW_KEY_S) == GLFW_PRESS)
        CamPos -= cameraspeed * CamUp;
    if (glfwGetKey(pWwindow, GLFW_KEY_A) == GLFW_PRESS)
        CamPos -= glm::normalize(glm::cross(CamFront, CamUp)) * cameraspeed;
    if (glfwGetKey(pWwindow, GLFW_KEY_D) == GLFW_PRESS)
        CamPos += glm::normalize(glm::cross(CamFront, CamUp)) * cameraspeed;
}

void Render::Setup(int Grid_Num, double Range_Min, double Range_Max, std::vector<double> &Potential)
{
    FDM_Solver solver(Grid_Num, Range_Min, Range_Max);
    this->Potential = Potential;

    solution = solver.Get_Solution(true, Potential);

    x = Potential::XaxisGenerator(Grid_Num, Range_Min, Range_Max);
    std::vector<double> y(solution[1].second.data(), solution[1].second.data() + solution[1].second.size());

    //if y.size ! = x.size error throw

    std::vector<Eigen::Vector2d> GraphPlot = SplinePoints(Grid_Num, 10, x, y);
    graph.setup(GraphPlot, std::make_shared<Shader>(shader));

    std::vector<Eigen::Vector2d> PotentialPlot = SplinePoints(Grid_Num, 10, x, Potential);
    potential.setup(PotentialPlot, std::make_shared<Shader>(shader));

    grid.Setup(std::make_shared<Shader>(shader), Range_Min * 2, Range_Max * 2,
               static_cast<int>(Range_Max - Range_Min) * 2);


    //packet generation
    wavePacket.PacketGeneration(Grid_Num, Range_Min, Range_Max, wavePacketData.mu, wavePacketData.sigma, wavePacketData.k_freq);
    auto data = wavePacket.GetDrawingData(10);
    packet.setup(data, std::make_shared<Shader>(shader));

    this->Grid_Num = Grid_Num;
    this->Range_Min = Range_Min;
    this->Range_Max = Range_Max;
}

void Render::Draw(Color GraphColor, Color GridColor)
{
    double tmp = 0.0;
    double time = 0.0;
    unsigned int frames = 0;
    PotentialType currentPotential = HarmonicOscillator;

    std::future<std::vector<std::pair<double, Eigen::VectorXd>>> future;

    bool buttonPressed = false;

    FDM_Solver solver(Grid_Num, Range_Min, Range_Max);

    while (!glfwWindowShouldClose(window))
    {
        processinput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double currentTime = glfwGetTime();

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

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(400, 300));

        ImGuiIO &io = ImGui::GetIO();
        io.FontGlobalScale = 1.5f;

        ImGui::Begin("window", nullptr,
                     ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        if (ImGui::BeginCombo("Eigenvalues", std::to_string(selecteditem).c_str()))
        {
            int previousSelectedItem = selecteditem;

            for (int i = 0; i < solution.size(); i++)
            {
                bool isSelected = (i == selecteditem);
                if (ImGui::Selectable(std::to_string(i).c_str(), isSelected))
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
                std::vector<double> y(solution[selecteditem].second.data(),
                                      solution[selecteditem].second.data() + solution[selecteditem].second.size());

                std::vector<Eigen::Vector2d> GraphPlot = SplinePoints(y.size(), 10, x, y);

                graph.Update(GraphPlot);
            }

            ImGui::EndCombo();
        }

        ImGui::Checkbox("time dependant", &check);

        if (check)
        {
            graph.TimePropagate(solution[selecteditem].first / solution[1].first, glfwGetTime());
        }
        else
        {
            graph.TimePropagate(0, 0);
        }

        ImGui::Checkbox("Wave Packet Draw", &wavePacketData.wavePacketDraw);
        if (wavePacketData.wavePacketDraw)
        {
            wavePacketSettingsGUI(wavePacketData, wavePacket, packet, Range_Min, Range_Max, Grid_Num);
            wavePacketPropagationGUI(wavePacketData, wavePacket, packet, Potential);
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0, 300));
        ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::Begin("window4", nullptr,
                     ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        if (ImGui::BeginCombo("Potential Type", GetPotentialName(currentPotential).c_str()))
        {
            if (ImGui::Selectable("Harmonic Oscillator", currentPotential == HarmonicOscillator))
            {
                currentPotential = HarmonicOscillator;
            }
            if (ImGui::Selectable("Free Space", currentPotential == FreeSpace))
            {
                currentPotential = FreeSpace;
            }
            if (ImGui::Selectable("Infinite Well", currentPotential == InfiniteWell))
            {
                currentPotential = InfiniteWell;
            }
            if (ImGui::Selectable("Finite Well", currentPotential == FiniteWell))
            {
                currentPotential = FiniteWell;
            }
            if (ImGui::Selectable("DiracDelta", currentPotential == DiracDelta))
            {
                currentPotential = DiracDelta;
            }
            ImGui::EndCombo();
        }

        if (currentPotential == InfiniteWell)
        {
            handleInfiniteWellGUI(infiniteWellData, Grid_Num, Range_Min, Potential, buttonPressed);
        }

        if (currentPotential == DiracDelta)
        {
            handleDiracDeltaGUI(diracDeltaData, Grid_Num, Range_Min, Potential, buttonPressed);
        }

        if (currentPotential == FiniteWell)
        {
            handleFiniteWellGUI(finiteSquareWellData, Grid_Num, Range_Min, Potential, buttonPressed);
        }

        if (currentPotential == HarmonicOscillator)
        {
            handleHarmonicOscillatorGUI(harmonicOscillatorData, Grid_Num, Range_Min, x, Potential, buttonPressed);
        }

        if (buttonPressed)
        {
            future = std::async(std::launch::async, &FDM_Solver::Get_Solution, &solver, true, Potential);

            buttonPressed = false;
        }

        using namespace std::chrono_literals;
        if (future.valid() && future.wait_for(0s) == std::future_status::ready)
        {
            solution = future.get();

            std::vector<double> y(solution[1].second.data(),
                                  solution[1].second.data() + solution[1].second.size());

            if (currentPotential == DiracDelta || currentPotential == InfiniteWell)
            {
                std::vector<Eigen::Vector2d> GraphPlot = LinearSpline(2, x, Potential);
                potential.Update(GraphPlot);
            }
            else
            {
                std::vector<Eigen::Vector2d> PotentialPlot = SplinePoints(Grid_Num, 10, x, Potential);
                potential.Update(PotentialPlot);
            }
        }

        if (ImGui::Button("Reset potential to zero"))
        {
            for (auto &z: Potential)
                z = 0.0;

            std::vector<Eigen::Vector2d> PotentialPlot = SplinePoints(Grid_Num, 10, x, Potential);
            potential.Update(PotentialPlot);
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        frames++;
        if (currentTime - lastTime > 1.0)
        {
            lastTime = glfwGetTime();
            std::cout << "FPS: " << frames << "\n";
            frames = 0;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}