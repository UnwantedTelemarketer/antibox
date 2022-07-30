#pragma once
#include "engine.h"
#include "app.h"

//PRINTSCREEN creates an auto reference to the current window (screen) and displays the framebuffer that it gets onto an ImGui Image tab.
#define PRINTSCREEN auto& window = *Engine::Instance().GetWindow(); ImGui::Image((void*)window.GetFramebuffer()->GetTextureID(), { ImGui::GetWindowSize().x , ImGui::GetWindowSize().y });

#define TO_CHARARR(number) std::to_string(number).c_str()


//To be implemented in clients app
//client returns pointer to instance of class derived from app
//ownership of returned pointer belongs to antibox, and will be managed as such

//example:
//class ClientApp : public antibox::App {};
//antibox::App* CreateApp() { return new ClientApp(); }
antibox::App* CreateApp();

int main() {
	antibox::App* app = CreateApp();
	antibox::Engine& engine = antibox::Engine::Instance();
	//antibox::Console& ConsoleIO = antibox::Console::Instance();

	engine.Run(app);
	delete app;
	return 0;
}

namespace Input {

	//Returns true if the key is pressed, false if not.
	int KeyDown(int keycode) { return glfwGetKey(antibox::Engine::Instance().GetWindow()->glfwin(), keycode); }
	//Returns true if the mouse button is pressed, false if not.
	int MouseButtonDown(int mouseButton) { return glfwGetMouseButton(antibox::Engine::Instance().GetWindow()->glfwin(), mouseButton); }
}