#include "Game.h"
#define CURRENTWINDOW Engine::Instance().GetWindow()->glfwin()

using namespace antibox;

class Caves : public App {
private:
	WindowProperties GetWindowProperties() {
		WindowProperties props;
		props.w = 1280;
		props.h = 720;
		props.vsync = 1;
		return props;
	}
	float tickRateVisual;
public:
	//UI stuff
	bool statsOpen, debugOpen;
	std::string openClose;
	//Game Stuff
	GameManager game;
	Entity zombo = { 100, "Zombie" };
	Player& player = game.mPlayer;
	float& health = game.mPlayer.health;
	Map& map = game.mainMap;

	void Init() override {
		health = 100.0f;
		statsOpen = false;
		openClose = "Open Stats";
		game.Setup(10, 10, 1);
		game.SpawnEnemy(&zombo);
	}
	void Update() {
		game.UpdateEntities();
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
		else if (Input::KeyDown(KEY_P))
		{
			debugOpen = !debugOpen;
		}
	}

	void ImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		//------Map-------
		ImGui::Begin("Map");
		for (int i = 0; i < CHUNK_WIDTH; i++) {
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				switch (game.mainMap.modifiedChunk.localCoords[i][j]) {
				case 0:
					ImGui::Text(ENT_PLAYER);
					break;
				case 2:
					ImGui::TextColored(ImVec4(0, 1, 1, 1), TILE_WATER);
					break;
				case 3:
					ImGui::TextColored(ImVec4(0, 1, 0, 1), TILE_FLOWER);
					break;
				case 35:
					ImGui::TextColored(ImVec4(1, 1, 0, 1), ENT_HUMAN);
					break;
				case 36:
					ImGui::TextColored(ImVec4(0.75f, 0, 0, 1), ENT_ZOMBIE);
					break;
				default:
					ImGui::TextColored(ImVec4(0, 1, 0, 1), TILE_GRASS);
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
		ImGui::End();
		//------Stats------
		if(statsOpen){
			ImGui::Begin("Stats");
				ImGui::Text("Health");
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor::HSV(0.0f,1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.0f, 1.0f, 0.5f));
				ImGui::ProgressBar(game.mPlayer.health / 100, ImVec2(0.0f, 0.0f));
				ImGui::PopStyleColor(2);

				if (player.coveredIn == 1) {
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(1.5f, 1.0f, 1.0f));
					ImGui::Text("Soaked!");
					ImGui::PopStyleColor(1);
				}
				ImGui::Text(std::to_string(Engine::Instance().getFPS()).c_str());
			ImGui::End();
		}
		//------NPC------
		if (game.NearNPC()) {
			ImGui::Begin("Current NPC");
				ImGui::Text(zombo.name);
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor::HSV(0.0f, 1.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(0.0f, 1.0f, 0.5f));
				ImGui::ProgressBar(zombo.health / 100, ImVec2(0.0f, 0.0f));
				ImGui::PopStyleColor(2);
				ImGui::Text("");
				ImGui::Text("Graaah.");
			ImGui::End();
		}
		//------DEBUG------
		if (debugOpen) {
			ImGui::Begin("Debug Window");
			//FPS
			ImGui::Text(("FPS: " + std::to_string(Engine::Instance().getFPS())).c_str());
			//ms left until next tick
			ImGui::Text(("Milliseconds until next update: " + std::to_string(game.GetTick())).c_str());
			//Display a bar until next tick
			ImGui::ProgressBar(game.GetTick() / game.TickRate(), ImVec2(0.0f, 0.0f));
			//set tickrate
			ImGui::InputFloat("New Tickrate", &tickRateVisual, 0.1f, 10);
			if (ImGui::Button("Change Tickrate")) {
				game.SetTick(tickRateVisual);
			}

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
		
		mTex = std::make_shared<Texture>("nothing");

		cookie = Factory::CreateSprite(pos);
	}

	void Update() override 
	{
		if (Input::KeyHeldDown(KEY_W)) 
		{
			cookie->Move({ 0.0f, -0.025f });
		}
		if (Input::KeyHeldDown(KEY_S))
		{
			cookie->Move({ 0.0f, 0.025f });
		}
		if (Input::KeyHeldDown(KEY_A))
		{
			cookie->Move({ -0.025f, 0.0f });
		}
		if (Input::KeyHeldDown(KEY_D))
		{
			cookie->Move({ 0.025f, 0.0f });
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
	return new Caves();
}