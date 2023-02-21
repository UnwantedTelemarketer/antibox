#include "map.h"
#include "items.h"

class GameManager {
private:
	float tickRate;
	double tickCount;
	bool forwardTime = true;
	std::vector<std::string> missMessages = { "blank", "You swing at nothing and almost fall over.", "You miss.", "You don't hit anything." };
public:
	Map mainMap;
	Player mPlayer;
	std::vector<std::string> actionLog;
	std::vector<std::string> possibleNames = {"John", "Zombie"};
	float worldTime = 1.f;

	double GetTick() { return (tickRate - tickCount); }
	float TickRate() { return tickRate; }
	void SetTick(float secs) { tickRate = secs * 1000; }


	void Setup(int x, int y, float tick);

	void UpdateEntities();
	void UpdateTick();

	void DoBehaviour(Entity* ent);

	void AttemptAttack(Entity* ent);

	//bool NearNPC() { return mainMap.NearNPC(mPlayer); }

	void SpawnEntity(Entity* curNPC);

	void MovePlayer(int dir);

	void SetTile(Vector2_I tile, int newTile);

	std::string GetTileChar(Tile tile);

	std::string GetTileChar(Vector2_I tile);

	ImVec4 GetTileColor(Tile tile);
	ImVec4 GetTileColor(Vector2_I tile);
};




void GameManager::Setup(int x, int y, float tick) {
	tickRate = tick * 1000;
	mPlayer.coords.x = x;
	mPlayer.coords.y = y;
	mainMap.CreateMap();
	mainMap.CreateStarterChunk(mPlayer);
}

void GameManager::DoBehaviour(Entity* ent)
{
	int dir = Math::RandInt(8);
	switch (ent->b)
	{
	case Wander:
	case Slow_Wander:
		switch (dir)
		{
		case 1:
			ent->coords.x++;
			break;
		case 2:
			ent->coords.x--;
			break;
		case 3:
			ent->coords.y++;
			break;
		case 4:
			ent->coords.y--;
			break;
		case 5:
			ent->coords.x++;
			ent->coords.y++;
			break;
		case 6:
			ent->coords.x--;
			ent->coords.y++;
			break;
		case 7:
			ent->coords.x++;
			ent->coords.y--;
			break;
		case 8:
			ent->coords.x--;
			ent->coords.y--;
			break;
		}
	}
}

void GameManager::SpawnEntity(Entity* ent) {
	ent->health = Math::RandNum(100);
	ent->coords.x = 10;
	ent->coords.y = 15;
	//if(curNPC->name == "") curNPC->name = Math::RandString(possibleNames);
	mainMap.CurrentChunk().entities.push_back(ent);
	ent->index = mainMap.CurrentChunk().entities.size() - 1;
}

void GameManager::MovePlayer(int dir) {
	switch (dir) {
	case 1:
		mainMap.MovePlayer(mPlayer.coords.x, mPlayer.coords.y - 1, &mPlayer, &actionLog);
		break;
	case 2:
		mainMap.MovePlayer(mPlayer.coords.x, mPlayer.coords.y + 1, &mPlayer, &actionLog);
		break;
	case 3:
		mainMap.MovePlayer(mPlayer.coords.x - 1, mPlayer.coords.y, &mPlayer, &actionLog);
		break;
	case 4:
		mainMap.MovePlayer(mPlayer.coords.x + 1, mPlayer.coords.y, &mPlayer, &actionLog);
		break;
	default:
		break;
	}
	//if (mainMap.NearNPC(player)) { Math::PushBackLog(&actionLog, "Howdy!"); }
}

void GameManager::UpdateTick() {
	tickCount += antibox::Engine::Instance().deltaTime();
	if (tickCount >= tickRate)
	{
		mPlayer.thirst -= 0.075f;
		mPlayer.hunger -= 0.05f;
		if (mPlayer.thirst < 0) { mPlayer.health -= 0.5f; mPlayer.thirst = 0; }
		if (mPlayer.hunger < 0) { mPlayer.health -= 0.5f; mPlayer.hunger = 0; }

		/*if (forwardTime) { worldTime += 0.5; }
		else { worldTime -= 0.5; }
		if (worldTime > 7 || worldTime <= 1) { forwardTime = !forwardTime; }*/

		UpdateEntities();
	}
}

void GameManager::UpdateEntities() {
	
		mainMap.ClearEntities(mPlayer);
		tickCount = 0;
		for (int i = 0; i < mainMap.CurrentChunk().entities.size(); i++)
		{
			if (mainMap.CurrentChunk().entities[i]->aggressive)
			{
				AttemptAttack(mainMap.CurrentChunk().entities[i]);
			}

			if (Math::RandInt(10) >= 4) { continue; }

			DoBehaviour(mainMap.CurrentChunk().entities[i]);


			mainMap.CheckBounds(mainMap.CurrentChunk().entities[i]);

		}
		mainMap.PlaceEntities(mPlayer);
}

void GameManager::AttemptAttack(Entity* ent)
{
	if (Vector2_I{ ent->coords.x + 1, ent->coords.y} == mPlayer.coords ||
		Vector2_I{ ent->coords.x - 1, ent->coords.y} == mPlayer.coords || 
		Vector2_I{ ent->coords.x, ent->coords.y + 1} == mPlayer.coords || 
		Vector2_I{ ent->coords.x, ent->coords.y - 1} == mPlayer.coords)
	{
		Math::PushBackLog(&actionLog, "Zombie bites you for 10 damage!");
		mPlayer.health -= 10;
	}
}

void GameManager::SetTile(Vector2_I tile, int newTile) {
	mainMap.CurrentChunk().localCoords[tile.x][tile.y].id = newTile;
}

std::string GameManager::GetTileChar(Tile tile) {
	
	if (tile.entity != nullptr) 
	{
		switch (tile.entity->entityID) 
		{
		case ID_ZOMBIE:
			return ENT_ZOMBIE;
			break;
		case ID_CHICKEN:
			return ENT_CHICKEN;
			break;
		}
	}
	switch (tile.id) {
	case 0:
		return TILE_NULL;
		break; 
	case 1:
		return TILE_GRASS;
		break;
	case 2:
		return TILE_DIRT;
		break;
	case 3:
		return TILE_FLOWER;
		break;
	case 4:
		return TILE_FIRE;
		break;
	default:
		return TILE_NULL;
		break;
	}
}

std::string GameManager::GetTileChar(Vector2_I tile) { return GameManager::GetTileChar(mainMap.CurrentChunk().localCoords[tile.x][tile.y]); }

ImVec4 GameManager::GetTileColor(Tile tile) {
	ImVec4 color;
	if (tile.entity != nullptr)
	{
		switch (tile.entity->entityID)
		{
		case ID_ZOMBIE:
			color = { 1,0,0,1 };
			goto dimming;
			break;
		case ID_CHICKEN:
			color = { 1,1,1,1 };
			goto dimming;
			break;
		}
	}

	switch (tile.liquid) {
	case water:
		color = { 0.25, 0.25, 1, 1 };
		goto dimming;
		break;
	case nothing:
		break;
	}

	switch (tile.id) {
	case 0:
		color = {0.75, 0.75, 0.75, 1};
		break;
	case 2:
		color = { 1, 0.45, 0, 1 };
		break;
	case 3:
		color = { 0.65, 1, 0.1, 1 };
		break;
	case 4:
		color = { 1, 0.35, 0, 1 };
		break;
	case 35:
		color = { 1, 1, 0, 1 };
		break;
	case 36:
		color = { 0.75, 0, 0, 1 };
		break;
	case 37:
		color = { 1, 1, 1, 1 };
		break;
	default:
		color = { 0, 1, 0, 1 };
		break;
	}

	dimming:
	color.x /= worldTime;
	color.y /= worldTime;
	color.z /= worldTime;

	return color;
}
ImVec4 GameManager::GetTileColor(Vector2_I tile) { return GameManager::GetTileColor(mainMap.CurrentChunk().localCoords[tile.x][tile.y]); }
