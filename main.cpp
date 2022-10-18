#include "Game.h"
#define CURRENTWINDOW Engine::Instance().GetWindow()->glfwin()

using namespace antibox;

class Caves : public App {
private:
	WindowProperties GetWindowProperties() {
		WindowProperties props;
		props.w = 1280;
		props.h = 720;
		return props;
	}
public:
	//UI stuff
	bool statsOpen;
	std::string openClose;
	//Game Stuff
	GameManager game;
	NPC johnNpc = { 100, 15, "John" };
	Player& player = game.mainPlayer;
	float& health = game.mainPlayer.health;
	Map& map = game.mainMap;

	void Init() override {
		health = 100.0f;
		statsOpen = false;
		openClose = "Open Stats";
		game.Setup(10, 10);
		game.SpawnEnemy(&johnNpc);
	}
	void Update() {
		if (Input::KeyDown(KEY_UP)) {
			game.MovePlayer(MAP_UP);
		}
		else if (Input::KeyDown(KEY_DOWN)) {
			game.MovePlayer(MAP_DOWN);
		}
		else if (Input::KeyDown(KEY_LEFT)) {
			game.MovePlayer(MAP_LEFT);
		}
		else if (Input::KeyDown(KEY_RIGHT)) {
			game.MovePlayer(MAP_RIGHT);
		}
	}

	void ImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		//------Map-------
		ImGui::Begin("Map");
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 30; j++) {
				switch (game.mainMap.testChunk.coords[i][j]) {
				case 0:
					ImGui::Text("@");
					break;
				case 2:
					ImGui::TextColored(ImVec4(0, 1, 1, 1), "~");
					break;
				case 3:
					ImGui::TextColored(ImVec4(1, 0, 1, 1), "*");
					break;
				case 35:
					ImGui::Text("#");
					break;
				default:
					ImGui::Text(".");
					break;
				}
				ImGui::SameLine();
			}
			ImGui::Text("");
		}
		ImGui::End();
		//------Action Log----
		ImGui::Begin("Action Log");
			for (int i = 0; i < game.actionLog.size(); i++)
			{
				ImGui::Text(game.actionLog[i].c_str());
			}
		ImGui::End();
		//------Combat------
		ImGui::Begin("Interact");
		if (ImGui::Button(openClose.c_str())) {
			statsOpen = !statsOpen;
			openClose = statsOpen ? "Close Stats" : "Open Stats";
		}
		if (ImGui::Button("Attack")) {
			game.AttemptAttack();
		}
		ImGui::End();
		//------Stats------
		if(statsOpen){
			ImGui::Begin("Stats");
				ImGui::Text("Health");
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor::HSV(0.0f,1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.0f, 1.0f, 0.5f));
				ImGui::ProgressBar(game.mainPlayer.health / 100, ImVec2(0.0f, 0.0f));
				ImGui::PopStyleColor(2);

				if (player.coveredIn == 1) {
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(1.5f, 1.0f, 1.0f));
					ImGui::Text("Soaked!");
					ImGui::PopStyleColor(1);
				}

			ImGui::End();
		}
		//------NPC------
		if (game.NearNPC()) {
			ImGui::Begin("Current NPC");
				ImGui::Text(johnNpc.name.c_str());
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor::HSV(0.0f, 1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.0f, 1.0f, 0.5f));
				ImGui::ProgressBar(johnNpc.health / 100, ImVec2(0.0f, 0.0f));
				ImGui::PopStyleColor(2);
				ImGui::Text("");
				ImGui::Text("Howdy!");
			ImGui::End();
		}
	}
};

class Pong : public App
{
private:
	std::shared_ptr<GameObject> cookie;
	double mouseX, mouseY;
	std::shared_ptr<Texture> mTex;

public:
	WindowProperties GetWindowProperties() {
		WindowProperties props;
		props.cc = { 0.2f,0.2f,0.2f,1.f };
		props.w = 800;
		props.h = 600;
		return props;
	}


	void Init() override 
	{
		static float Vertices[]
		{
			 0.5f,  0.5f, 0.f,
			 0.5f, -0.5f, 0.f,
			-0.5f, -0.5f, 0.f,
			-0.5f,  0.5f, 0.f
		};

		static uint32_t Elements[]
		{
			0, 3, 1,
			1, 3, 2
		};

		static float texcoords[]
		{
			1.f, 1.f,
			1.f, 0.f,
			0.f, 0.f,
			0.f, 1.f,
		};

		static const char* DefaultVert = R"(
			#version 410 core
			layout (location = 0) in vec3 position;
			layout (location = 1) in vec2 texcoords;
			uniform mat4 model = mat4(1.0);
			out vec2 uvs;

			void main()
			{
				uvs = texcoords;
				gl_Position = model * vec4(position,1.0);
			}
		)";

		static const char* DefaultFrag = R"(
			#version 410 core
			out vec4 outColor;
			in vec2 uvs;

			uniform sampler2D tex;
			void main()
			{
				//outColor = vec4(1.0);
				outColor = texture(tex, uvs);
			}
		)";

		mTex = std::make_shared<Texture>("res/none");
		std::shared_ptr<antibox::Mesh> mesh = std::make_shared<antibox::Mesh>(&Vertices[0], 4, 3, &Elements[0], 6);
		std::shared_ptr<antibox::Shader> shader = std::make_shared<antibox::Shader>(DefaultVert, DefaultFrag);

		glm::vec2 size{ 0.5,0.5 };
		glm::vec2 pos{ 0.f,0.f };
		cookie = std::make_shared<antibox::GameObject>(mesh, shader, pos, size, mTex);
		
	}

	void Update() override 
	{
		cookie->Update();
	}

	void Render() override 
	{
		cookie->Render();
	}

	void ImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		if (ImGui::Begin("Game View"))
		{
			PRINTSCREEN;
		}
		ImGui::End();
	}

	void Shutdown() override {

	}
};

class DND : public App{

	int dice;
	int dice_amount;
	bool disadvantage;
	int dice_dis;
	

	WindowProperties GetWindowProperties() {
		WindowProperties props;
		props.w = 800;
		props.h = 600;
		props.title = "D&D Character Interaction";
		return props;
	}

	void Init() override
	{
		dice_amount = 20;
	}
	void Update() override
	{

	}
	void ImguiRender() override
	{
		ImGui::Begin("Roll Dice"); {
			if (ImGui::RadioButton("Two Dice", disadvantage))
			{
				disadvantage = !disadvantage;
			}
			ImGui::SliderInt(CHAR_ARRAY(dice_amount), &dice_amount, 0, 60);
			if (ImGui::Button(("Roll D" + std::to_string(dice_amount)).c_str()))
			{
				dice = Math::RandInt(dice_amount);
				dice_dis = Math::RandInt(dice_amount);
			}
			ImGui::Text(std::to_string(dice).c_str());
			if (disadvantage) {
				ImGui::Text(CHAR_ARRAY(dice_dis));
			}
		
		ImGui::End(); }

		ImGui::Begin("");
	}
};

antibox::App* CreateApp() {
	return new Pong();
}