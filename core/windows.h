#pragma once

#include "thread.h"
#include "gui/imgui/imgui.h"
#include "gui/imgui/backends/imgui_impl_glfw.h"
#include "gui/imgui/backends/imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


void glfw_error_callback(const int error, const char* description);



namespace sst::gui
{
	class windows : public threading::thread
	{
	public:
		using thread::thread;

		virtual void run_custom() = 0;

	private:
		bool setup() override;
		void run() override;
		void cleanup() override;

		GLFWwindow* window_{ nullptr };
		bool show_demo_window_{ true };
		bool show_another_window_{ false };
		ImVec4 clear_color_{ 0.45f, 0.55f, 0.60f, 1.00f };
	};
}
