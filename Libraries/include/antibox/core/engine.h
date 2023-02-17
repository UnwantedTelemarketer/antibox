#pragma once
#include "window.h"
#include "../managers/rendermanager.h"
#include "antibox/graphics/framebuffer.h"
#include "antibox/graphics/helpers.h"
#include "antibox/core/mathlib.h"


//================================================================================================================================================
#define MD_FLAG antibox::Engine::Instance().mouseDownFlag
#define KD_FLAG antibox::Engine::Instance().keyDownFlag
#define KD_CODE antibox::Engine::Instance().keyDownCode
//#define END_WINDOW_SHADER(window, shader, shad) shader.Delete(); shad.Delete(); glfwDestroyWindow(window); glfwTerminate(); //Closes the window and destroys it
//#define DEFAULT_FRAMEBUFFER(program) glBindFramebuffer(GL_FRAMEBUFFER, 0); program.Activate(); glBindVertexArray(rectVAO); glDisable(GL_DEPTH_TEST); glBindTexture(GL_TEXTURE_2D, fbTex); glDrawArrays(GL_TRIANGLES, 0, 6); // idk at this point
//#define READ_FROM_FILE(currChunkIDS, k, s) std::string xs = std::to_string(k); std::string ys = std::to_string(s); std::ifstream input_file("saves/worlds/chunks/" + (xs + ys)); while (input_file.get(byte)){ currChunkIDS.push_back(parseChartoInt(byte));}
//================================================================================================================================================


namespace antibox {
	class App;
	class Engine {
	public:
		unsigned int window_w, window_h; //Window height and width
		Window* window; //Reference to the window

		bool mouseDownFlag; //Flag used to see if a mouse button is held down, used in the antibox.h input section
		bool keyDownFlag; //Flag used to see if a key is held down
		int keyDownCode; //Variable holding the last pressed key

		static Engine& Instance(); //Singleton for engine
		~Engine(); //Destructor

		
		void End(); //Called when window is closed
		//void EndShader();
		//void AddToObjects(GameObject* go);

		void Run(App* app); //The constant loop every frame
		inline App& GetApp() { return *mApp; } //Returns App
		Window* GetWindow() { return window; } //Returns Window
		inline render::RenderManager& GetRenderManager() { return mRenderManager; } //Returns RenderManager

	private:

		App* mApp; //Pointer to the current app. There should only ever be one app.
		Engine();
		static Engine* mInstance; //Local reference to the engine singleton

		render::RenderManager mRenderManager; //RenderManager takes in Render Commands for rendering
		//std::vector<GameObject*> gos; //This is an array of gameobjects so everytime you make a new one, it adds it to here to be updated and rendered.

		double prevtime = 0.0; //all of this is for counting framerate if people want
		double crntTime = 0.0; 
		double timeDiff; 
		unsigned int counter = 0;

		void Update(); //Self Explanatory
		void Render();
		void Initialize();
	};
}
