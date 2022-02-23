#include "MagePCH.h"
#include "TrashTheCache.h"
#include "imgui.h"
#include "imgui_plot.h"
#include "algorithm"
#include <chrono>

void TrashTheCache::TrashWithInts()
{
    if (m_IntSamples > 0)
    {
        m_IntTimes.clear();

        for (int const stepSize : m_StepSizes)
        {
            long long average = 0;

            for (int k = 0; k < m_IntSamples; k++)
            {
                constexpr int size = 100000;
                int* ints{ new int[size]{} };

                auto start = std::chrono::high_resolution_clock::now();

                for (size_t j = 0; j < size; j += stepSize)
                {
                    ints[j] *= 2;
                }

                auto end = std::chrono::high_resolution_clock::now();
                average += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

                delete[] ints;
            }

            average /= m_IntSamples;
            m_IntTimes.push_back(float(average));
        }
    }
}

void TrashTheCache::DisplayIntsGraph()
{
    std::vector<float> floatStepSizes;
    for (int i : m_StepSizes)
    {
        floatStepSizes.push_back(float(i));
    }

    ImGui::PlotConfig conf;
    conf.values.xs = floatStepSizes.data(); // this line is optional
    conf.values.ys = m_IntTimes.data();
    conf.values.count = int(m_StepSizes.size());
    conf.values.color = ImColor(255, 128, 0);
    conf.scale.min = 0;
    conf.scale.max = *std::max_element(m_IntTimes.begin(), m_IntTimes.end());
    conf.tooltip.show = true;
    conf.tooltip.format = "stepsize: %.f\ntime: %.f";
    conf.grid_x.show = false;
    conf.grid_y.show = false;
    conf.frame_size = ImVec2(200, 100);
    conf.line_thickness = 2.f;

    ImGui::Plot("", conf);
}

void TrashTheCache::TrashWithObjects()
{
    if (m_ObjectSamples > 0)
    {
        m_ObjectTimes.clear();

        for (const int stepSize : m_StepSizes)
        {
            long long average = 0;

            for (int k = 0; k < m_ObjectSamples; k++)
            {
                constexpr int size = 100000;
                Object* objects{ new Object[size] {} };

                auto start = std::chrono::high_resolution_clock::now();

                for (size_t j = 0; j < size; j += stepSize)
                {
                    objects[j].ID *= 2;
                }

                auto end = std::chrono::high_resolution_clock::now();
                average += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

                delete[] objects;
            }

            average /= m_ObjectSamples;
            m_ObjectTimes.push_back(float(average));
        }
    }
}

void TrashTheCache::DisplayObjectsGraph()
{
    std::vector<float> floatStepSizes;
    for (int i : m_StepSizes)
    {
        floatStepSizes.push_back(float(i));
    }

    ImGui::PlotConfig conf;
    conf.values.xs = floatStepSizes.data(); // this line is optional
    conf.values.ys = m_ObjectTimes.data();
    conf.values.count = int(m_StepSizes.size());
    conf.values.color = ImColor(0, 255, 255);
    conf.scale.min = 0;
    conf.scale.max = *std::max_element(m_ObjectTimes.begin(), m_ObjectTimes.end());
    conf.tooltip.show = true;
    conf.tooltip.format = "stepsize: %.f\ntime: %.fns";
    conf.grid_x.show = false;
    conf.grid_y.show = false;
    conf.frame_size = ImVec2(200, 100);
    conf.line_thickness = 2.f;

    ImGui::Plot("", conf);
}

void TrashTheCache::TrashWithAltObjects()
{
    m_AltObjectTimes.clear();

    for (const int stepSize : m_StepSizes)
    {
        long long average = 0;

        for (int k = 0; k < m_AltObjectSamples; k++)
        {
            constexpr int size = 100000;
            AltObject* objects{ new AltObject[size]{} };

            auto start = std::chrono::high_resolution_clock::now();

            for (size_t j = 0; j < size; j += stepSize)
            {
                objects[j].ID *= 2;
            }

            auto end = std::chrono::high_resolution_clock::now();
            average += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

            delete[] objects;
        }

        average /= m_AltObjectSamples;
        m_AltObjectTimes.push_back(float(average));
    }
}

void TrashTheCache::DisplayAltObjectsGraph()
{
    std::vector<float> floatStepSizes;
    for (int i : m_StepSizes)
    {
        floatStepSizes.push_back(float(i));
    }

    ImGui::PlotConfig conf;
    conf.values.xs = floatStepSizes.data(); // this line is optional
    conf.values.ys = m_AltObjectTimes.data();
    conf.values.count = int(m_StepSizes.size());
    conf.values.color = ImColor(0, 255, 128);
    conf.scale.min = 0;
    conf.scale.max = *std::max_element(m_AltObjectTimes.begin(), m_AltObjectTimes.end());
    conf.tooltip.show = true;
    conf.tooltip.format = "stepsize: %.f\ntime: %.fns";
    conf.grid_x.show = false;
    conf.grid_y.show = false;
    conf.frame_size = ImVec2(200, 100);
    conf.line_thickness = 2.f;

    ImGui::Plot("", conf);
}

void TrashTheCache::DisplayCombinedObjectsGraph()
{
    std::vector<float> floatStepSizes;
    for (int i : m_StepSizes)
    {
        floatStepSizes.push_back(float(i));
    }

    const float* combinedTimes[] = { m_AltObjectTimes.data(), m_ObjectTimes.data() };
    ImU32 colors[2] = { ImColor(0, 255, 128), ImColor(0, 255, 255) };

    ImGui::PlotConfig conf;
    conf.values.xs = floatStepSizes.data(); // this line is optional
    conf.values.ys_list = combinedTimes;
    conf.values.ys_count = 2;
    conf.values.count = int(m_StepSizes.size());
    conf.values.colors = colors;
    conf.scale.min = 0;
    conf.scale.max = *std::max_element(m_ObjectTimes.begin(), m_ObjectTimes.end());
    conf.tooltip.show = true;
    conf.tooltip.format = "stepsize: %.f\ntime: %.fns";
    conf.grid_x.show = false;
    conf.grid_y.show = false;
    conf.frame_size = ImVec2(200, 100);
    conf.line_thickness = 2.f;

    ImGui::Plot("", conf);
}


void TrashTheCache::Update()
{
    // With integers
    ImGui::Begin("Exercise 1");

    ImGui::InputInt("# samples", &m_IntSamples);

    if (ImGui::Button("Trash the chache"))
        TrashWithInts();

    if (!m_IntTimes.empty())
        DisplayIntsGraph();

    ImGui::End();

    // With objecs
    ImGui::Begin("Exercise 2");
    
    ImGui::InputInt("# samples", &m_ObjectSamples);

    if (ImGui::Button("Trash the chache with Object"))
        TrashWithObjects();

    if (!m_ObjectTimes.empty())
        DisplayObjectsGraph();

    if (ImGui::Button("Trash the cache with AltObject"))
        TrashWithAltObjects();

    if (!m_AltObjectTimes.empty())
        DisplayAltObjectsGraph();

    if (!m_ObjectTimes.empty() && !m_AltObjectTimes.empty())
    {
        ImGui::Text("Combined:");
        DisplayCombinedObjectsGraph();
    }

    ImGui::End();
}