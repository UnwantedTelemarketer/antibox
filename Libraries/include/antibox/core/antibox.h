#pragma once
#include "engine.h"
#include "app.h"

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

	engine.Run(app);
	delete app;
	return 0;
}

namespace Input {
	//Returns true if the key is pressed, false if not.
	inline int KeyDown(int keycode) { return glfwGetKey(antibox::Engine::Instance().GetWindow()->glfwin(), keycode); }
}