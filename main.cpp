#include "Game.h"

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
					ImGui::TextColored(ImVec4(0, 0, 1, 1), "~");
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
		if(Input::KeyDown(KEY_UP)) { game.MovePlayer(MAP_UP); }
		else if (Input::KeyDown(KEY_DOWN)) { game.MovePlayer(MAP_DOWN); }
		else if (Input::KeyDown(KEY_LEFT)) { game.MovePlayer(MAP_LEFT); }
		else if (Input::KeyDown(KEY_RIGHT)) { game.MovePlayer(MAP_RIGHT); }
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
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(1.7f, 1.0f, 1.0f));
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
	std::shared_ptr<GameObject> mLeftPaddle, mRightPaddle;
public:
	
	WindowProperties GetWindowProperties() {
		WindowProperties props;
		props.cc = { 0.85f,0.15f,0.15f,1.f };
		return props;
	}

	void Init() override 
	{
		mLeftPaddle = factory::CreateSprite({ -0.85f, 0.f });
		mRightPaddle = factory::CreateSprite({ 0.85f, 0.f });
	}
	void Update() override 
	{
		mLeftPaddle->Update();
		mRightPaddle->Update();
	}
	void Render() override 
	{
		mLeftPaddle->Render();
		mRightPaddle->Render();
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

antibox::App* CreateApp() {
	return new Caves();
}