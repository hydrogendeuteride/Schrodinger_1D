#ifndef SCHRODINGER_1D_IMGUICONTROLS_H
#define SCHRODINGER_1D_IMGUICONTROLS_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Potentials.h"
namespace
{
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

    struct InfiniteWellData
    {
        double wellStart;
        double wellEnd;
    };

    struct DiracDeltaData
    {
        double wellStart;
    };

    struct FiniteSquareWellData
    {
        double wellStart;
        double wellEnd;
        double wellDepth;
    };

    struct HarmonicOscillatorData
    {
        double k;
    };

    void
    updatePotentialWithFunction(int Grid_Num, std::vector<double> &Potential, const std::vector<double> &newPotential,
                                bool &buttonPressed)
    {
        for (int i = 0; i < Grid_Num; ++i)
        {
            Potential[i] += newPotential[i];
        }
        buttonPressed = true;
    }

    void handleInfiniteWellGUI(InfiniteWellData &data, int Grid_Num, double Range_Min, std::vector<double> &Potential,
                               bool &buttonPressed)
    {
        auto start_temp = static_cast<float>(data.wellStart);
        auto end_temp = static_cast<float>(data.wellEnd);

        if (ImGui::SliderFloat("Well Start", &start_temp, -6.0f, 6.0f) ||
            ImGui::SliderFloat("Well End", &end_temp, -6.0f, 6.0f))
        {
            if (end_temp <= start_temp)
                end_temp = start_temp + 0.1f;

            data.wellStart = static_cast<double>(start_temp);
            data.wellEnd = static_cast<double>(end_temp);
        }
        if (ImGui::Button("Add"))
        {
            updatePotentialWithFunction(Grid_Num, Potential,
                                        Potential::InfiniteSquareWell(Grid_Num, data.wellStart, data.wellEnd,
                                                                      Range_Min),
                                        buttonPressed);
        }
    }

    void handleDiracDeltaGUI(DiracDeltaData &data, int Grid_Num, double Range_Min, std::vector<double> &Potential,
                             bool &buttonPressed)
    {
        auto start_temp = static_cast<float>(data.wellStart);

        if (ImGui::SliderFloat("Start", &start_temp, -6.0f, 6.0f))
        {
            data.wellStart = static_cast<double>(start_temp);
        }

        if (ImGui::Button("Add"))
        {
            updatePotentialWithFunction(Grid_Num, Potential,
                                        Potential::DiracDelta(Grid_Num, data.wellStart, Range_Min),
                                        buttonPressed);
        }
    }

    void handleFiniteWellGUI(FiniteSquareWellData &data, int Grid_Num, double Range_Min, std::vector<double> &Potential,
                             bool &buttonPressed)
    {
        auto start_temp = static_cast<float>(data.wellStart);
        auto end_temp = static_cast<float>(data.wellEnd);
        auto depth_temp = static_cast<float>(data.wellDepth);

        if (ImGui::SliderFloat("Well Start", &start_temp, -6.0f, 6.0f) ||
            ImGui::SliderFloat("Well End", &end_temp, -6.0f, 6.0f) ||
            ImGui::SliderFloat("well Depth", &depth_temp, -10.0f, 0.0f))
        {
            if (end_temp <= start_temp)
                end_temp = start_temp;

            data.wellStart = static_cast<double>(start_temp);
            data.wellEnd = static_cast<double>(end_temp);
            data.wellDepth = static_cast<double>(depth_temp);
        }

        if (ImGui::Button("Add"))
        {
            updatePotentialWithFunction(Grid_Num, Potential,
                                        Potential::FiniteSquareWell(Grid_Num, data.wellStart, data.wellEnd,
                                                                    data.wellDepth,
                                                                    Range_Min),
                                        buttonPressed);
        }
    }

    void
    handleHarmonicOscillatorGUI(HarmonicOscillatorData &data, int Grid_Num, double Range_Min, std::vector<double> &x,
                                std::vector<double> &Potential,
                                bool &buttonPressed)
    {
        float k_temp = static_cast<float>(data.k);
        if (ImGui::SliderFloat("K (spring constant)", &k_temp, 0.1, 10.0))
        {
            data.k = static_cast<double>(k_temp);
        }
        if (ImGui::Button("Add"))
        {
            updatePotentialWithFunction(Grid_Num, Potential,
                                        Potential::HarmonicOscillatorPotential(Grid_Num, data.k, x),
                                        buttonPressed);
        }
    }
};

#endif //SCHRODINGER_1D_IMGUICONTROLS_H
