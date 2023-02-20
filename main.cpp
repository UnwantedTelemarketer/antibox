#include "GameStuff/game.h"
#define CURRENTWINDOW Engine::Instance().GetWindow()->glfwin()
#define UNIFONT "c:\\Users\\Thomas Andrew\\AppData\\Local\\Microsoft\\Windows\\Fonts\\Unifont.ttf"
#define CASCADIA "c:\\Windows\\Fonts\\CascadiaCode.ttf"

#include <conio.h>

using namespace antibox;


class Caves : public App {
private:
	WindowProperties GetWindowProperties() { 
		WindowProperties props;
		
		props.imguiProps = { true, true, false, CASCADIA};
		props.w = 1280;
		props.h = 720;
		props.vsync = 1;
		return props;
	}

	float tickRateVisual, lastFPS;
	int counter = 0;
public:
	//UI stuff
	bool statsOpen, debugOpen, interacting, itemMenu, navInv;
	Tile* selectedTile = nullptr;
	int currentItemIndex = 0;
	std::string openClose;
	//Game Stuff
	GameManager game;
	Inventory pInv;
	Entity zombo = { 100, "Zombie", ID_ZOMBIE, Stationary, true };
	Entity chicken = { 25, "Chicken", ID_CHICKEN, Wander, false };
	Player& player = game.mPlayer;
	float& health = game.mPlayer.health;
	Map& map = game.mainMap;

	void Init() override {
		health = 100.0f;
		statsOpen = true;
		openClose = "Close Stats";
		navInv = false;
		game.Setup(10, 10, 0.5f);
		game.SpawnEntity(&zombo);
		game.SpawnEntity(&chicken);

		pInv.clothes = { 0.75, 0.45, 0.15 };
	}

	void Update() {
		game.UpdateEntities();
		if (Input::KeyDown(KEY_UP)) {
			if (interacting)
			{
				selectedTile = { map.TileAtPos(Vector2_I{ player.coords.y - 1, player.coords.x }) };
				interacting = false;
				return;
			}
			else if (navInv) 
			{
				currentItemIndex--;
				currentItemIndex = currentItemIndex < 0 ? 0 : currentItemIndex;
				return;
			}
			selectedTile = nullptr;
			game.MovePlayer(MAP_UP);
		}
		else if (Input::KeyDown(KEY_DOWN)) {
			if (interacting)
			{
				selectedTile = { map.TileAtPos(Vector2_I{ player.coords.y + 1, player.coords.x }) };
				interacting = false;
				return;
			}
			else if (navInv)
			{
				currentItemIndex++;
				if(currentItemIndex > pInv.items.size() - 1) currentItemIndex = pInv.items.size() - 1;
				return;
			}
			selectedTile = nullptr;
			game.MovePlayer(MAP_DOWN);
		}
		else if (Input::KeyDown(KEY_LEFT)) {
			if (interacting) 
			{
				selectedTile = { map.TileAtPos(Vector2_I{ player.coords.y, player.coords.x - 1}) };
				interacting = false;
				return;
			}
			selectedTile = nullptr;
			game.MovePlayer(MAP_LEFT);
		}
		else if (Input::KeyDown(KEY_RIGHT)) {
			if (interacting)
			{
				selectedTile = { map.TileAtPos(Vector2_I{ player.coords.y, player.coords.x + 1}) };
				interacting = false;
				return;
			}
			selectedTile = nullptr;
			game.MovePlayer(MAP_RIGHT);
		}
		else if (Input::KeyDown(KEY_P))
		{
			debugOpen = !debugOpen;
		}
		else if (Input::KeyDown(KEY_I))
		{
			navInv = !navInv;
			itemMenu = navInv;
		}
		else if (Input::KeyDown(KEY_M))
		{
			pInv.AddItem({ "Canteen (Empty)", 1, false, true, 0 });
		}
		else if (Input::KeyDown(KEY_L))
		{
			pInv.AddItem({ "Grass", 12, true, false, 1 });
		}
		else if (Input::KeyDown(KEY_E))
		{
			if (!navInv) 
			{
				Math::PushBackLog(&game.actionLog, "Which direction will you interact with?");
				interacting = true;
			}
		}
	}

	void ImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		//------Map-------
		ImGui::Begin("Map");
		ImGui::PushFont(Engine::Instance().getFont());
		for (int i = 0; i < CHUNK_WIDTH; i++) {
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				if (Vector2_I{ player.coords.x,player.coords.y } == Vector2_I{ j,i })
				{
					ImGui::Text("@");
				}
				else {
					ImGui::TextColored(game.GetTileColor(map.CurrentChunk().localCoords[i][j]),
									   game.GetTileChar(map.CurrentChunk().localCoords[i][j]).c_str());
				}
				ImGui::SameLine();
			}
			ImGui::Text("");
		}
		ImGui::PopFont();
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
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4{1,0,0,1});
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.45,0,0,1 });
				ImGui::ProgressBar(game.mPlayer.health / 100, ImVec2(0.0f, 0.0f));
				ImGui::PopStyleColor(2);


				ImGui::Text("Thirst");
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4{ 0,0.5,1,1 });
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0.45,1 });
				ImGui::ProgressBar(game.mPlayer.thirst / 100, ImVec2(0.0f, 0.0f));
				ImGui::PopStyleColor(2);

				if (player.coveredIn == water) {
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(1.5f, 1.0f, 1.0f));
					ImGui::Text("Soaked!");
					ImGui::PopStyleColor(1);
				}
			ImGui::End();
		}

		//------Inventory------
		ImGui::Begin("Inventory");
		for (int i = 0; i < pInv.items.size(); i++)
		{
			if (i == currentItemIndex && navInv) {
				ImGui::Text("> "); ImGui::SameLine();
			}
			ImGui::Text(pInv.items[i].name.c_str()); ;
			if (pInv.items[i].count > 0) 
			{
				ImGui::SameLine(); ImGui::Text(CHAR_ARRAY(pInv.items[i].count));
			}
		}
		ImGui::End();

		//------Interaction------
		if (selectedTile != nullptr) 
		{
			ImGui::Begin("Selected Block");
			ImGui::TextColored(game.GetTileColor(*selectedTile), game.GetTileChar(*selectedTile).c_str());
			if (ImGui::Button("Collect")) {
				if (pInv.AttemptCollect(selectedTile)) 
				{
					selectedTile->liquid = nothing;
					Math::PushBackLog(&game.actionLog, "You collect the water off the ground.");
				}
				else {

					Math::PushBackLog(&game.actionLog, "You can't collect that.");
				}
				selectedTile = nullptr;
			}
			ImGui::End();
		}
		//------item------
		if (itemMenu)
		{
			ImGui::Begin("Current Item");
			ImGui::Text(pInv.items[currentItemIndex].name.c_str());

			
			if (!pInv.items[currentItemIndex].stackable) { ImGui::TextColored(ImVec4{1,0,0,1},"[DOES NOT STACK]"); }
			
			if (pInv.items[currentItemIndex].holdsLiquid) {
				ImGui::Text("Liquid:");
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4{ 0,0.5,1,1 });
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0.45,1 });

				ImGui::ProgressBar(pInv.items[currentItemIndex].liquidAmount / 100, ImVec2(0.0f, 0.0f));
				ImGui::PopStyleColor(2);
			}
			if (pInv.items[currentItemIndex].stackable) {
				ImGui::Text("[ Holding x");
				ImGui::SameLine(); 
				ImGui::Text(std::to_string(pInv.items[currentItemIndex].count).c_str());
				ImGui::SameLine();
				ImGui::Text("]");
			}
			

			ImGui::End();
		}

		//------DEBUG------
		if (debugOpen) {
			ImGui::Begin("Debug Window");
			//FPS
			if (counter == 30) { lastFPS = Engine::Instance().getFPS(); counter = 0; }
			else { counter++; }
			ImGui::Text(("FPS: " + std::to_string(lastFPS)).c_str());
			//ms left until next tick
			ImGui::Text(("Time until next update:"));
			//Display a bar until next tick
			ImGui::ProgressBar(game.GetTick() / game.TickRate(), ImVec2(0.0f, 0.0f));
			//set tickrate
			ImGui::InputFloat("New Tickrate", &tickRateVisual, 0.5f, 10);
			if (ImGui::Button("Change Tickrate")) {
				game.SetTick(tickRateVisual);
			}
			ImGui::Text(("Local X: " + std::to_string(player.coords.x)).c_str());
			ImGui::Text(("Local Y: " + std::to_string(player.coords.y)).c_str());
			
			ImGui::Text(("Global X: " + std::to_string(map.c_glCoords.x)).c_str());
			ImGui::Text(("Global Y: " + std::to_string(map.c_glCoords.y)).c_str());
			
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