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

class Tanks : public App
{
private:
	std::shared_ptr<GameObject> cookie;
	double mouseX, mouseY;
	std::shared_ptr<Texture> mTex;
	std::shared_ptr<antibox::Mesh> mMesh;
	std::shared_ptr<antibox::Shader> mShader;

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

		glm::vec2 size{ 0.5f,0.5f };
		glm::vec2 pos{ 0.f,0.f };
		
		//mTex = std::make_shared<Texture>("nothing");

		cookie = Factory::CreateSprite(pos);
	}

	void Update() override 
	{
		if (Input::KeyDown(KEY_I)) 
		{
			cookie->Move({ 0.0f, -0.1f });
		}
		else if (Input::KeyDown(KEY_K))
		{
			cookie->Move({ 0.0f, 0.1f });
		}
		else if (Input::KeyDown(KEY_J))
		{
			cookie->Move({ -0.1f, 0.0f });
		}
		else if (Input::KeyDown(KEY_L))
		{
			cookie->Move({ 0.1f, 0.0f });
		}
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
	return new Tanks();
}