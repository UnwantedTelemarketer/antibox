#include "GameStuff/game.h"
#define CURRENTWINDOW Engine::Instance().GetWindow()->glfwin()
#define UNIFONT "c:\\Users\\Thomas Andrew\\AppData\\Local\\Microsoft\\Windows\\Fonts\\Unifont.ttf"
#define CASCADIA "c:\\Windows\\Fonts\\CascadiaCode.ttf"

#ifndef maybe
#error Cmon man, define "maybe".
#endif


using namespace antibox;

class Caves : public App {
private:
	WindowProperties GetWindowProperties() { 
		WindowProperties props;
		
		props.imguiProps = { true, true, false, UNIFONT };
		props.w = 1280;
		props.h = 720;
		props.vsync = 1;
		return props;
	}

	float tickRateVisual, lastFPS;
	int counter = 0;
public:
	//UI stuff
	bool statsOpen, debugOpen, interacting, itemMenu, navInv, useBool;
	Tile* selectedTile = nullptr;
	int currentItemIndex = 0;
	std::string openClose;

	//Game Stuff
	GameManager game;
	Inventory pInv;
	Player& player = game.mPlayer;
	float& health = game.mPlayer.health;
	Map& map = game.mainMap;

	void Init() override {
		health = 100.0f;
		statsOpen = true;
		openClose = "Close Stats";
		navInv = false;
		game.Setup(10, 10, 0.5f);

		pInv.clothes = { 0.75, 0.45, 0.15 };
		pInv.AddItem(canteen);
		pInv.AddItem(bandage);
		pInv.AddItem(jar);

		GenericConsoleOut<bool>(maybe, WARNING);
	}

	void Update() {
		game.UpdateTick();
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
				else if (map.effectLayer.localCoords[i][j] == 15) 
				{
					ImGui::TextColored(ImVec4{ 1,0,1,1 }, "X");
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
		ImGui::PushFont(Engine::Instance().getFont());
			for (int i = 0; i < game.actionLog.size(); i++)
			{
				ImGui::Text(game.actionLog[i].c_str());
			}
		ImGui::PopFont();
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
				ImGui::TextColored(ImVec4{0.85, 0.15, 0.15, 1},"Health");
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4{0.85,0,0,1});
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.5,0,0,1 });
				ImGui::ProgressBar(game.mPlayer.health / 100, ImVec2(0.0f, 0.0f));
				ImGui::PopStyleColor(2);


				ImGui::TextColored(ImVec4{ 0.15, 0.65, 1, 1 }, "Thirst");
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4{ 0,0.5,1,1 });
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0.25,0.5,1 });
				ImGui::ProgressBar(game.mPlayer.thirst / 100, ImVec2(0.0f, 0.0f));
				ImGui::PopStyleColor(2);


				ImGui::TextColored(ImVec4{ 1, 0.6, 0.15, 1 }, "Hunger");
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4{ 0.8,0.5,0,1 });
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.4,0.25,0,1 });
				ImGui::ProgressBar(game.mPlayer.hunger / 100, ImVec2(0.0f, 0.0f));
				ImGui::PopStyleColor(2);

				if (player.coveredIn == water) {
					ImGui::TextColored(ImVec4{ 0, 0.6, 1, 1 }, "Soaked!");
				}
			ImGui::End();
		}

		//------Inventory------
		ImGui::Begin("Inventory");
		for (int i = 0; i < pInv.items.size(); i++)
		{
			if (pInv.items[i].count <= 0) { continue; }
			if (i == currentItemIndex && navInv) {
				ImGui::Text("> "); ImGui::SameLine();
			}
			ImGui::Text(pInv.items[i].name.c_str()); ;
			if (pInv.items[i].count > 1) 
			{
				ImGui::SameLine(); ImGui::Text("x");
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
					selectedTile->id = selectedTile->replacementID;
					selectedTile->collectible = false;
					Math::PushBackLog(&game.actionLog, "You collect off the ground.");
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

			if (ImGui::Button("Use"))
			{
				useBool = !useBool;
			}

			if (useBool)
			{
				ImGui::Text("On what?");
				if (ImGui::Button("Consume")) 
				{
					if (pInv.AttemptAction(consume, &pInv.items[currentItemIndex], &player))
					{
						if (pInv.items[currentItemIndex].consumable) { pInv.items[currentItemIndex].count--; }
						Math::PushBackLog(&game.actionLog, pInv.items[currentItemIndex].consumeTxt);
					}
					else
					{
						Math::PushBackLog(&game.actionLog, "You can't consume " + pInv.items[currentItemIndex].name + ".");
					}
					useBool = !useBool;
				}
				if (ImGui::Button("Body (Self)"))
				{
					if (pInv.AttemptAction(use, &pInv.items[currentItemIndex], &player))
					{
						if (pInv.items[currentItemIndex].consumable) { pInv.items[currentItemIndex].count--; }
						Math::PushBackLog(&game.actionLog, pInv.items[currentItemIndex].useTxt);
					}
					else 
					{
						Math::PushBackLog(&game.actionLog, "You can't use " + pInv.items[currentItemIndex].name + ".");
					}
					useBool = !useBool;
				}
			}

			ImGui::End();
		}

		//------DEBUG------
		if (debugOpen) {
			ImGui::Begin("Debug Window");
			//FPS
			if (counter == 30) { lastFPS = Engine::Instance().getFPS(); counter = 0; }
			else { counter++; }
			ImGui::Text(("Current World Time: " + std::to_string(game.worldTime)).c_str());
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

antibox::App* CreateApp() {
	return new Caves();
}