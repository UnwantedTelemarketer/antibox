#include "engine.h"
#include "app.h"
#include "../graphics/shader.h"

namespace antibox
{
	Engine& Engine::Instance() 
	{
		if (!mInstance) {
			mInstance = new Engine(); //If we haven't set the engine instance yet, set it.
		}

		return *mInstance;
	}
	//singleton
	Engine* Engine::mInstance = nullptr;

	Engine::Engine() {
		mApp = nullptr;
		window_w = 1280;
		window_h = 720;
		window = new Window(window_w, window_h, "name");
	}

	

	void Engine::Initialize() { //Do all the initialization
		srand((unsigned)time(NULL)); //for rng
		WindowProperties props = mApp->GetWindowProperties();
		window->init(props);
		mRenderManager.Init();
		mApp->Init();
	}

	//void Engine::AddToObjects(GameObject* go) {
	//	gos.push_back(go);
	//}

	void Engine::Run(App* app) { //This is what loops forever until the window is closed
		if (mApp == nullptr) { mApp = app; }//If we dont have an app, set the private app to the one submitted from wherever run is called.
		else { return; } //if there is no app anywhere, just dont run
		Initialize(); //
		/*for (int i = 0; i < gos.size(); i++) //Update each sprite after the user wants to mess with it
		{
			gos[i]->CreateSprite();
		}*/
		while (!glfwWindowShouldClose(window->glfwin())) //This is the window loop from GLFW.
		{
			Update(); //Run the Update function
			Render(); //Run the Render Function
		}
		End(); //Once the user closes the window, run End
	}

	void Engine::Update() {
		window->BeginRender(); //Start the rendering from window

		crntTime = glfwGetTime();
		timeDiff = crntTime - prevtime;
		counter++; 
		if (timeDiff >= 1.0 / 60.0) {
			fps = (1.0 / timeDiff) * counter;
			ms = (timeDiff / counter) * 1000;
			prevtime = crntTime; 	
			counter = 0;
		} //report the framerate and ms between frames

		mApp->Update(); //users update function
	}

	void Engine::Render() {
		glfwPollEvents(); //Take in mouse and keyboard inputs
		
		mApp->Render(); //Users render function.
		/*for (int i = 0; i < sprites.size(); i++) //Render each sprite added to screen
		{
			sprites[i]->RenderSprite();
		}*/
		window->EndRender(); //Window end render.
	}

	void Engine::End() {
		//client shutdown (?)
		mApp->Shutdown(); //Users shutdown function
		mRenderManager.Shutdown();
		glfwDestroyWindow(window->glfwin());
		glfwTerminate();
	}

	Engine::~Engine() {
		delete window;
	}
}
