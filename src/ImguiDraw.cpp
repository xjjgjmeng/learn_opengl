#include "ImguiDraw.h"

#include <string>

#include "Common.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace
{
    void cameraSetup()
    {
        if (ImGui::TreeNode("Camera"))
        {
            auto& camera = GlNs::gData.camera;
            ImGui::Checkbox("autoRotation", &GlNs::gData.camera.autoRotation);
            ImGui::DragFloat3("pos", &camera.camera.Position[0], .005f);
            ImGui::DragFloat3("front", &camera.camera.Front[0], .005f);
            ImGui::DragFloat3("up", &camera.camera.Up[0], .005f);
            ImGui::TreePop();
        }
    }

    void lampSetup()
    {
        if (ImGui::TreeNode("Lamp"))
        {
            auto& lamp = GlNs::gData.lamp;

            if (ImGui::TreeNode("Point Light"))
            {
                ImGui::Checkbox("attenuation", &lamp.attenuation);
                ImGui::TreePop();
            }

            ImGui::Checkbox("autoRotation", &lamp.autoRotation);
            ImGui::Checkbox("softEdges", &lamp.softEdges);
            //ImGui::Checkbox("directional", &lamp.directional);
            ImGui::Checkbox("dir", &lamp.dir); ImGui::SameLine();
            ImGui::Checkbox("point", &lamp.point); ImGui::SameLine();
            ImGui::Checkbox("spot", &lamp.spot);
#if 0
            auto& lampType = lamp.type;
            ImGui::RadioButton("Directional", reinterpret_cast<int*>(&lampType), static_cast<int>(LightType::Directional)); ImGui::SameLine();
            ImGui::RadioButton("Point", reinterpret_cast<int*>(&lampType), static_cast<int>(LightType::Point)); ImGui::SameLine();
            ImGui::RadioButton("Spot", reinterpret_cast<int*>(&lampType), static_cast<int>(LightType::Spot));
#endif
            if (ImGui::TreeNode("点光源"))
            {
                for (auto i = 0; i < std::size(GlNs::gData.pointLightPositions); ++i)
                {
                    ImGui::DragFloat3(std::to_string(i).data(), &GlNs::gData.pointLightPositions[i][0], .05f);
                }
                ImGui::TreePop();
            }
            ImGui::DragFloat("shininess", &GlNs::gData.shininess, 2, 2, 256);
            ImGui::TreePop();
        }
    }
}

void imguiInit()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(GlNs::gData.window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void imguiDraw()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (GlNs::gData.show_demo_window)
        ImGui::ShowDemoWindow(&GlNs::gData.show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        //static float f = 0.0f;
        //static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::Checkbox("enableMouseMove", &GlNs::gData.enableMouseMove);
        ImGui::Checkbox("showSmile", &GlNs::gData.showSmile);
        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &GlNs::gData.show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &GlNs::gData.show_another_window);

        //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&GlNs::gData.clear_color); // Edit 3 floats representing a color
        ImGui::ColorEdit4("lamp color", (float*)&GlNs::gData.lamp_color); // Edit 3 floats representing a color

        ImGui::SliderFloat("angle", &GlNs::gData.ang, -360, 360);
        ImGui::SliderFloat("nearP", &GlNs::gData.nearP, 0, GlNs::gData.farP);
        ImGui::SliderFloat("farP", &GlNs::gData.farP, GlNs::gData.nearP, 300.f);
        ImGui::SliderFloat("FOV(degrees)", &GlNs::gData.camera.camera.Zoom, 0.f, 100.f);


        ImGui::Checkbox("depth testing", &GlNs::gData.depthTesting);
        ImGui::Checkbox("more cube", &GlNs::gData.showMore);
        ImGui::Checkbox("rotate by time", &GlNs::gData.rotateByTime);

        cameraSetup();
        lampSetup();

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (GlNs::gData.show_another_window)
    {
        ImGui::Begin("Another Window", &GlNs::gData.show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            GlNs::gData.show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}