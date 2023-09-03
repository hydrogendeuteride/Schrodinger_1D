#include "Render.h"
#include <numeric>
#include <cstdlib>

std::string GetPotentialName(PotentialType potential)
{
    switch (potential)
    {
        case HarmonicOscillator:
            return "Harmonic Oscillator";
        case FreeSpace:
            return "Free Space";
        case FiniteWell:
            return "Finite Well";
        case InfiniteWell:
            return "Infinite Well";
        case DiracDelta:
            return "Dirac Delta";
        default:
            return "Harmonic Oscillator";
    }
}

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
    wavePacket.PacketGeneration(Grid_Num, Range_Min, Range_Max, mu, sigma, k_freq);
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

        ImGui::Checkbox("Wave Packet Draw", &wavePacketDraw);
        if (wavePacketDraw)
        {
            auto mu_temp = static_cast<float>(mu);
            auto sigma_temp = static_cast<float>(sigma);
            auto k_freq_temp = static_cast<float>(k_freq);

            if (ImGui::SliderFloat("mu", &mu_temp, Range_Min, Range_Max) ||
                ImGui::SliderFloat("sigma", &sigma_temp, 0.01, 2.0) ||
                ImGui::SliderFloat("k", &k_freq_temp, PI, 128 * PI))
            {
                mu = static_cast<double>(mu_temp);
                sigma = static_cast<double>(sigma_temp);
                k_freq = static_cast<double>(k_freq_temp);
            }

            if(ImGui::Button("Generate Packet"))
            {
                wavePacket.PacketGeneration(Grid_Num, Range_Min, Range_Max, mu, sigma, k_freq);
                packet.Update(wavePacket.GetDrawingData(10));
                packet.draw(Red);
            }

            ImGui::Checkbox("Wave Packet Propagate", &wavePacketPropagate);
            if (wavePacketPropagate)
            {
                tmp += 0.0001;
                wavePacket.TimePropagate(tmp, Potential);
                packet.Update(wavePacket.GetDrawingData(10));
                packet.draw(Red);
            }
            else
                packet.draw(Red);
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
            ImGui::EndCombo();
        }

        if (currentPotential == HarmonicOscillator)
        {
            float k_temp = static_cast<float>(k);
            if (ImGui::SliderFloat("K (spring constant)", &k_temp, 0.1, 10.0))
            {
                k = static_cast<double>(k_temp);
            }
            if (ImGui::Button("Add"))
            {
                auto t = Potential::HarmonicOscillatorPotential(Grid_Num, k, x);
                for (int i = 0; i < Grid_Num; ++i)
                {
                    Potential[i] += t[i];
                }

                FDM_Solver solver(Grid_Num, Range_Min, Range_Max);

                solution = solver.Get_Solution(true, Potential);
                std::vector<double> y(solution[1].second.data(), solution[1].second.data() + solution[1].second.size());

                std::vector<Eigen::Vector2d> GraphPlot = SplinePoints(Grid_Num, 10, x, y);
                potential.Update(GraphPlot);
            }
            auto newGraph = SplinePoints(Potential.size(), 10, x, Potential);
            potential.Update(newGraph);
        }
        if (currentPotential == InfiniteWell)
        {
            auto start_temp = static_cast<float>(wellStart);
            auto end_temp = static_cast<float>(wellEnd);

            if (ImGui::SliderFloat("Well Start", &start_temp, -6.0f, 6.0f) ||
                ImGui::SliderFloat("Well End", &end_temp, -6.0f, 6.0f))
            {
                if (wellEnd <= wellStart)
                    wellEnd = wellStart;

                wellStart = static_cast<double>(start_temp);
                wellEnd = static_cast<double>(end_temp);
            }
            if (ImGui::Button("Add"))
            {
                auto t = Potential::InfiniteSquareWell(Grid_Num, wellStart, wellEnd, Range_Min);
                for (int i = 0; i < Grid_Num; ++i)
                {
                    Potential[i] += t[i];
                }

                FDM_Solver solver(Grid_Num, Range_Min, Range_Max);

                solution = solver.Get_Solution(true, Potential);
                std::vector<double> y(solution[1].second.data(), solution[1].second.data() + solution[1].second.size());

                std::vector<Eigen::Vector2d> GraphPlot = LinearSpline(2, x, Potential);
                potential.Update(GraphPlot);
            }
        }

        if (currentPotential == FiniteWell)
        {
            auto start_temp = static_cast<float>(wellStart);
            auto end_temp = static_cast<float>(wellEnd);
            auto depth_temp = static_cast<float>(wellDepth);

            if (ImGui::SliderFloat("Well Start", &start_temp, -6.0f, 6.0f) ||
                ImGui::SliderFloat("Well End", &end_temp, -6.0f, 6.0f) ||
                ImGui::SliderFloat("well Depth", &depth_temp, -10.0f, 0.0f))
            {
                if (wellEnd <= wellStart)
                    wellEnd = wellStart;

                wellStart = static_cast<double>(start_temp);
                wellEnd = static_cast<double>(end_temp);
                wellDepth = static_cast<double>(depth_temp);
            }
            if (ImGui::Button("Add"))
            {
                auto t = Potential::FiniteSquareWell(Grid_Num, wellStart, wellEnd, wellDepth, Range_Min);
                for (int i = 0; i < Grid_Num; ++i)
                {
                    Potential[i] += t[i];
                }

                FDM_Solver solver(Grid_Num, Range_Min, Range_Max);

                solution = solver.Get_Solution(true, Potential);
                std::vector<double> y(solution[1].second.data(), solution[1].second.data() + solution[1].second.size());

                std::vector<Eigen::Vector2d> GraphPlot = LinearSpline(2, x, Potential);
                potential.Update(GraphPlot);
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

