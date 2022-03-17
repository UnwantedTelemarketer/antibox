#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace antibox 
{
	struct ImguiWindowProperties
	{
		bool MoveTitleBarOnly = true;
		bool DockingEnabled = true;
		bool IsViewportEnabled = false;
	};

	class ImguiWindow 
	{
	public:
		ImguiWindow() {}
		~ImguiWindow() {}

		void Create(const ImguiWindowProperties& props);
		void Shutdown();

		bool WantCaptureMouse();
		bool WantCaptureKeyboard();

		void BeginRender();
		void EndRender();
	};
}