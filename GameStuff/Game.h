#include "map.h"

class GameManager {
private:
	float tickRate;
	double tickCount;
	std::vector<std::string> missMessages = { "blank", "You swing at nothing and almost fall over.", "You miss.", "You don't hit anything." };
public:
	Map mainMap;
	Player mPlayer;
	std::vector<std::string> actionLog;
	std::vector<std::string> possibleNames = {"John", "Zombie"};

	double GetTick() { return (tickRate - tickCount); }
	float TickRate() { return tickRate; }
	void SetTick(float secs) { tickRate = secs * 1000; }

	void Setup(int x, int y, float tick);

	void UpdateEntities();

	void DoBehaviour(Entity* ent);

	void AttemptAttack(Entity* ent);

	bool NearNPC() { return mainMap.NearNPC(mPlayer); }

	void SpawnEntity(Entity* curNPC);

	void MovePlayer(int dir);
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

void GameManager::UpdateEntities() {
	tickCount += antibox::Engine::Instance().deltaTime();
	if (tickCount >= tickRate)
	{
		mPlayer.coveredIn = 0;
		mainMap.ClearEntities(mPlayer);
		tickCount = 0;
		for (int i = 0; i < mainMap.CurrentChunk().entities.size(); i++)
		{
			if (Math::RandInt(10) >= 3) { continue; }

			DoBehaviour(mainMap.CurrentChunk().entities[i]);

			if (mainMap.CurrentChunk().entities[i]->aggressive)
			{
				AttemptAttack(mainMap.CurrentChunk().entities[i]);
			}

			mainMap.CheckBounds(mainMap.CurrentChunk().entities[i]);

		}
		mainMap.PlaceEntities(mPlayer);
	}
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