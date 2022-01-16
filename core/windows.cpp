#include "stdafx.h"
#include "windows.h"
#include "logger.h"

#include "gui/imgui/imgui.h"
#include "gui/imgui/backends/imgui_impl_glfw.h"
#include "gui/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "trait_util/class_instance_counter.h"


void glfw_error_callback(const int error, const char* description)
{
    log_debug << "Glfw Error : " << error << "desc : " << description;
}

namespace sst::gui
{
	bool windows::setup()
	{
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
		{
			return false;
		}

		constexpr auto glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		// Create window with graphics context
		window_ = glfwCreateWindow(1280, 720, "sst test client", nullptr, nullptr);
		if (window_ == nullptr)
		{
			return false;
		}
        
        glfwMakeContextCurrent(window_);
        glfwSwapInterval(1); // Enable vsync


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window_, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		return true;
	}

	void windows::run()
	{
        while (!glfwWindowShouldClose(window_))
        {
            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
            // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            /*if (show_demo_window_)
            {
                ImGui::ShowDemoWindow(&show_demo_window_);
            }*/

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            //{
            //    static float f = 0.0f;
            //    static int counter = 0;

            //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            //    ImGui::Checkbox("Demo Window", &show_demo_window_);      // Edit bools storing our window open/close state
            //    ImGui::Checkbox("Another Window", &show_another_window_);

            //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            //    ImGui::ColorEdit3("clear color", (float*)&clear_color_); // Edit 3 floats representing a color

            //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            //    {
            //        counter++;
            //    }

            //    ImGui::SameLine();
            //    ImGui::Text("counter = %d", counter);

            //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            //    ImGui::End();
            //}

            


            // 3. Show another simple window.
            if (show_another_window_)
            {
                ImGui::Begin("Another Window", &show_another_window_);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window_ = false;
                ImGui::End();
            }

            {
                ImGui::Begin("Control Pad");

                if (ImGui::CollapsingHeader("Memory"))
                {
                    ImGui::Text("ClassCounter");               // Display some text (you can use a format strings too)
                    ImGui::SameLine();
                    if (ImGui::Button("Detail"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    {
                    }

                    for (const auto& [name, count, size] : sst::class_instance_counter::infos)
                    {
                        if (name.empty())
                        {
                            continue;
                        }

                        std::ostringstream ss;
                        ss << "class name : " << name.data() << " | " << std::to_string(count.load()) << " | " << "size : " << std::to_string(size);
                        ImGui::Text(ss.str().c_str());
                    }
                }
                if (ImGui::CollapsingHeader("Performance"))
                {
                    ImGui::Text("CPU Usage");               // Display some text (you can use a format strings too)
                    ImGui::SameLine();
                    if (ImGui::Button("Show"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    {


                    }
                }

                ImGui::End();
            }

            run_custom();

            // Rendering
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window_, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color_.x * clear_color_.w, clear_color_.y * clear_color_.w, clear_color_.z * clear_color_.w, clear_color_.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window_);
        }
	}

	void windows::cleanup()
	{
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window_);
        glfwTerminate();
	}
}
