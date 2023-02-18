#pragma once
#include "antibox/core/antibox.h"
#include "antibox/managers/factory.h"
#include "entities.h"
#include "inventory.h"
#include <algorithm>

#define MAP_UP 1
#define MAP_DOWN 2
#define MAP_LEFT 3
#define MAP_RIGHT 4
#define CHUNK_WIDTH 30
#define CHUNK_HEIGHT 30
#define MAP_HEIGHT 5
#define MAP_WIDTH 5

struct Player {
	float health = 0;
	std::string name = "Blank";
	int xCoord = 0;
	int yCoord = 0;
	int coveredIn = 0; // 0 = nothing, 1 = water, 2 = mud
	int ticksCovered = 0;
};


struct Chunk {
	Vector2_I globalChunkCoord;
	int localCoords[CHUNK_WIDTH][CHUNK_HEIGHT];
	std::vector<Entity*> entities;
};
struct T_Chunk //we dont need the current chunk copy to have an entity list
{
	int localCoords[CHUNK_WIDTH][CHUNK_HEIGHT];
};

struct World
{
	Chunk map[MAP_WIDTH][MAP_HEIGHT];
};


class Map {
public:
	Vector2_I c_glCoords{ 2, 3 };
	T_Chunk entityLayer; //is the original chunk with entities placed over it
	World gameMap; //keeps the original generated map so that tiles walked over wont be erased

	void CreateMap() 
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				gameMap.map[x][y].globalChunkCoord.x = x;
				gameMap.map[x][y].globalChunkCoord.y = y;
				BuildChunk(&gameMap.map[x][y]);
			}
		}
	}

	Chunk& CurrentChunk() {
		return gameMap.map[c_glCoords.x][c_glCoords.y];
	}

	void MovePlayer(int x, int y, Player* p, std::vector<std::string>* actionLog) {
		ClearEntities(*p);
		p->xCoord = x; p->yCoord = y;
		CheckBounds(p);
		//if (CurrentChunk().localCoords[y][x] == 2) { p->coveredIn = 1; Math::PushBackLog(actionLog, "You are now wet."); }
		PlaceEntities(*p);
	}

	void CheckBounds(Player* p) { //check if we need to move to the next chunk
		bool changed = false;
		if (p->xCoord > CHUNK_WIDTH - 1){
			changed = true;
			p->xCoord = 0;
			c_glCoords.x++;
		}
		else if (p->xCoord < 0) {
			changed = true;
			p->xCoord = CHUNK_WIDTH - 1;
			c_glCoords.x--;
		}
		else if (p->yCoord > CHUNK_HEIGHT - 1) {
			changed = true;
			p->yCoord = 0;
			c_glCoords.y++;
		}
		else if (p->yCoord < 0) {
			changed = true;
			p->yCoord = CHUNK_HEIGHT - 1;
			c_glCoords.y--;
		}
		if (changed) {
			//gameMap.map = gameMap.map[currentGlobalCoords.x][currentGlobalCoords.y];
			SetShownChunk();
		}
	}
	void CheckBounds(Entity* p) { //check if we need to move to the next chunk
		bool changed = false;
		if (p->xCoord > CHUNK_WIDTH - 1) {
			changed = true;
			p->xCoord = 0;
			c_glCoords.x++;
		}
		else if (p->xCoord < 0) {
			changed = true;
			p->xCoord = CHUNK_WIDTH - 1;
			c_glCoords.x--;
		}
		else if (p->yCoord > CHUNK_HEIGHT - 1) {
			changed = true;
			p->yCoord = 0;
			c_glCoords.y++;
		}
		else if (p->yCoord < 0) {
			changed = true;
			p->yCoord = CHUNK_HEIGHT - 1;
		}
		if (changed) {
			//gameMap.map = gameMap.map[currentGlobalCoords.x][currentGlobalCoords.y];
			SetShownChunk();
		}
	}

	void SetShownChunk() { //update the chunk we see to the current one
		for (int i = 0; i < CHUNK_WIDTH; i++) {
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				entityLayer.localCoords[i][j] = gameMap.map[c_glCoords.x][c_glCoords.y].localCoords[i][j];
			}
		}
	}

	void CreateStarterChunk(Player p) {
		SetShownChunk();
	}

	void BuildChunk(Chunk* chunk) {
		for (int i = 0; i < CHUNK_WIDTH; i++) {
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				int currentTile = Math::RandNum(10);
				if (currentTile > 9) { chunk->localCoords[i][j] = 2; }
				else if (currentTile > 7) { chunk->localCoords[i][j] = 3; }
				else { chunk->localCoords[i][j] = 1; }
			}
		}
	}

	bool NearNPC(Player p) {
		//check around player
		if (entityLayer.localCoords[p.yCoord + 1][p.xCoord] == '#' ||
			entityLayer.localCoords[p.yCoord - 1][p.xCoord] == '#' ||
			entityLayer.localCoords[p.yCoord][p.xCoord + 1] == '#' ||
			entityLayer.localCoords[p.yCoord][p.xCoord - 1] == '#') {
			return true;
		}
		return false;
	}

	void ClearEntities(Player p)
	{
		//go to the player and all entities and replace the original tile
		entityLayer.localCoords[p.yCoord][p.xCoord] = CurrentChunk().localCoords[p.yCoord][p.xCoord];
		for (int i = 0; i < CurrentChunk().entities.size(); i++)
		{
			Entity& curEn = *CurrentChunk().entities[i];
			entityLayer.localCoords[curEn.yCoord][curEn.xCoord] = CurrentChunk().localCoords[curEn.yCoord][curEn.xCoord];
		}
	}

	void PlaceEntities(Player p) 
	{
		//place the player and all entities on top of the tiles
		entityLayer.localCoords[p.yCoord][p.xCoord] = 0;
		int id;
		for (int i = 0; i < CurrentChunk().entities.size(); i++)
		{
			if (CurrentChunk().entities[i]->name == "Zombie") { id = 36; }
			else if (CurrentChunk().entities[i]->name == "Chicken") { id = 37; }
			else { id = 35; }
			entityLayer.localCoords[CurrentChunk().entities[i]->yCoord][CurrentChunk().entities[i]->xCoord] = id;
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
				if (Math::RandInt(10) >= 5)
				{
					mainMap.CurrentChunk().entities[i]->xCoord++;
				}
				else
				{
					mainMap.CurrentChunk().entities[i]->xCoord--;
				}
				if (Math::RandInt(10) >= 5)
				{
					mainMap.CurrentChunk().entities[i]->yCoord++;
				}
				else
				{
					mainMap.CurrentChunk().entities[i]->yCoord--;
				}
				
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