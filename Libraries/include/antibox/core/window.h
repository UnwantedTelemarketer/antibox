#pragma once
#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include <memory>
#include <glm/glm.hpp>
#include <string>
#include "antibox/core/imguiwindow.h"


namespace antibox {

	class Framebuffer;
	struct WindowProperties //Window Properties is how you edit width, height, title, and background color as well as editing ImGui Properties.
	{
		ImguiWindowProperties imguiProps;
		std::string title; // Title of the window.
		int w, h; //Width and height of the window.
		glm::ivec4 cc;

		WindowProperties();
	};

	class Window
	{
	public:

		Window(const unsigned int width, const unsigned int height, const char* windowName);

		bool init(const WindowProperties& props);
		void BeginRender();
		void EndRender();
		void GetEvents();
		glm::ivec2 GetSize();

		inline GLFWwindow* glfwin() { return Window::win; }
		inline ImguiWindow imwin() { return Window::mImguiWindow; }
		inline Framebuffer* GetFramebuffer() { return mFramebuffer.get(); }

	private:
		GLFWwindow* win;
		unsigned int width;
		unsigned int height;
		const char* windowName;
		ImguiWindow mImguiWindow;

		std::shared_ptr <Framebuffer> mFramebuffer;
	};
}
#endif // !WINDOW_CLASS_H
