#pragma once
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

	void Setup(int x, int y, float tick) {
		tickRate = tick * 1000;
		mPlayer.xCoord = x;
		mPlayer.yCoord = y;
		mainMap.CreateMap();
		mainMap.CreateStarterChunk(mPlayer);
	}

	void UpdateEntities() 
	{
		tickCount += antibox::Engine::Instance().deltaTime();
		if (tickCount >= tickRate)
		{
			mPlayer.coveredIn = 0;
			mainMap.ClearEntities(mPlayer);
			tickCount = 0;
			for (int i = 0; i < mainMap.CurrentChunk().entities.size(); i++)
			{
				if (Math::RandInt(10) >= 3) { continue; }
				int dir = Math::RandInt(8);
				switch (dir)
				{
				case 1:
					mainMap.CurrentChunk().entities[i]->xCoord++;
					break;
				case 2:
					mainMap.CurrentChunk().entities[i]->xCoord--;
					break;
				case 3:
					mainMap.CurrentChunk().entities[i]->yCoord++;
					break;
				case 4:
					mainMap.CurrentChunk().entities[i]->yCoord--;
					break;
				case 5:
					mainMap.CurrentChunk().entities[i]->xCoord++;
					mainMap.CurrentChunk().entities[i]->yCoord++;
					break;
				case 6:
					mainMap.CurrentChunk().entities[i]->xCoord--;
					mainMap.CurrentChunk().entities[i]->yCoord++;
					break;
				case 7:
					mainMap.CurrentChunk().entities[i]->xCoord++;
					mainMap.CurrentChunk().entities[i]->yCoord--;
					break;
				case 8:
					mainMap.CurrentChunk().entities[i]->xCoord--;
					mainMap.CurrentChunk().entities[i]->yCoord--;
					break;
				}
				mainMap.CheckBounds(mainMap.CurrentChunk().entities[i]);
			}
			mainMap.PlaceEntities(mPlayer);
		}
	}

	bool NearNPC() {
		return mainMap.NearNPC(mPlayer);
	}

	void SpawnEntity(Entity* curNPC) {
		curNPC->health = Math::RandNum(100);
		curNPC->xCoord = 10;
		curNPC->yCoord = 15;
		//if(curNPC->name == "") curNPC->name = Math::RandString(possibleNames);
		mainMap.CurrentChunk().entities.push_back(curNPC);
		curNPC->index = mainMap.CurrentChunk().entities.size() - 1;
	}

	void MovePlayer(int dir) {
		switch (dir) {
		case 1:
			mainMap.MovePlayer(mPlayer.xCoord, mPlayer.yCoord - 1, &mPlayer, &actionLog);
			break;
		case 2:
			mainMap.MovePlayer(mPlayer.xCoord, mPlayer.yCoord + 1, &mPlayer, &actionLog);
			break;
		case 3:
			mainMap.MovePlayer(mPlayer.xCoord - 1, mPlayer.yCoord, &mPlayer, &actionLog);
			break;
		case 4:
			mainMap.MovePlayer(mPlayer.xCoord + 1, mPlayer.yCoord, &mPlayer, &actionLog);
			break;
		default:
			break;
		}
		//if (mainMap.NearNPC(player)) { Math::PushBackLog(&actionLog, "Howdy!"); }
	}
};