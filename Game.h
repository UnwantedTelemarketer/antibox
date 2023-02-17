#pragma once
#include "antibox/core/antibox.h"
#include "antibox/managers/factory.h"
#include "entities.h"

#define MAP_UP 1
#define MAP_DOWN 2
#define MAP_LEFT 3
#define MAP_RIGHT 4
#define CHUNK_WIDTH 30
#define CHUNK_HEIGHT 30

struct Player {
	float health = 0;
	std::string name = "Blank";
	int xCoord = 0;
	int yCoord = 0;
	int coveredIn = 0; // 0 = nothing, 1 = water, 2 = mud
	int ticksCovered = 0;
};


struct Chunk {
	Vector2 globalChunkCoord;
	int localCoords[CHUNK_WIDTH][CHUNK_HEIGHT];
	std::vector<Entity*> entities;
};


class Map {
public:
	Chunk modifiedChunk;
	Chunk originalChunk; //keeps the original generated map so that tiles walked over wont randomize

	void MovePlayer(int x, int y, Player* p, std::vector<std::string>* actionLog) {
		ClearEntities(*p);
		p->xCoord = x; p->yCoord = y;
		if (originalChunk.localCoords[y][x] == 2) { p->coveredIn = 1; Math::PushBackLog(actionLog, "You are now wet."); }
		PlaceEntities(*p);
	}

	bool NearNPC(Player p) {
		//check around player
		if (modifiedChunk.localCoords[p.yCoord + 1][p.xCoord] == '#' ||
			modifiedChunk.localCoords[p.yCoord - 1][p.xCoord]  == '#' ||
			modifiedChunk.localCoords[p.yCoord][p.xCoord + 1]  == '#' ||
			modifiedChunk.localCoords[p.yCoord][p.xCoord - 1]  == '#') {
			return true;
		}
		return false;
	}

	void BuildInitialMap(Player p) {
		for (int i = 0; i < CHUNK_WIDTH; i++) {
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				int currentTile = Math::RandNum(10);
				if (currentTile > 9) { originalChunk.localCoords[i][j] = 2; }
				else if (currentTile > 7) { originalChunk.localCoords[i][j] = 3; }
				else { originalChunk.localCoords[i][j] = 1; }
				modifiedChunk.localCoords[i][j] = originalChunk.localCoords[i][j];
			}
		}
		modifiedChunk.localCoords[p.yCoord][p.xCoord] = 0;
	}

	void ClearEntities(Player p)
	{
		//go to the player and all entities and replace the original tile
		modifiedChunk.localCoords[p.yCoord][p.xCoord] = originalChunk.localCoords[p.yCoord][p.xCoord];
		for (int i = 0; i < originalChunk.entities.size(); i++)
		{
			Entity& curEn = *originalChunk.entities[i];
			modifiedChunk.localCoords[curEn.yCoord][curEn.xCoord] = originalChunk.localCoords[curEn.yCoord][curEn.xCoord];
		}
	}

	void PlaceEntities(Player p) 
	{
		//place the player and all entities on top of the tiles
		modifiedChunk.localCoords[p.yCoord][p.xCoord] = 0;
		int id;
		for (int i = 0; i < originalChunk.entities.size(); i++)
		{
			if (originalChunk.entities[i]->name == "Zombie") { id = 36; }
			else { id = 35; }
			modifiedChunk.localCoords[originalChunk.entities[i]->yCoord][originalChunk.entities[i]->xCoord] = id;
		}
	}
};

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
		mainMap.BuildInitialMap(mPlayer);
	}

	void UpdateEntities() 
	{
		tickCount += antibox::Engine::Instance().deltaTime();
		if (tickCount >= tickRate)
		{
			mPlayer.coveredIn = 0;
			mainMap.ClearEntities(mPlayer);
			tickCount = 0;
			for (int i = 0; i < mainMap.originalChunk.entities.size(); i++)
			{
				if (Math::RandInt(3) != 2) { continue; }
				if (Math::RandInt(10) >= 5)
				{
					mainMap.originalChunk.entities[i]->xCoord++;
				}
				else
				{
					mainMap.originalChunk.entities[i]->xCoord--;
				}
				if (Math::RandInt(10) >= 5)
				{
					mainMap.originalChunk.entities[i]->yCoord++;
				}
				else
				{
					mainMap.originalChunk.entities[i]->yCoord--;
				}
			}
			mainMap.PlaceEntities(mPlayer);
		}
	}

	bool NearNPC() {
		return mainMap.NearNPC(mPlayer);
	}

	void SpawnEnemy(Entity* curNPC) {
		curNPC->health = Math::RandNum(100);
		curNPC->xCoord = 10;
		curNPC->yCoord = 15;
		//if(curNPC->name == "") curNPC->name = Math::RandString(possibleNames);
		mainMap.originalChunk.entities.push_back(curNPC);
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