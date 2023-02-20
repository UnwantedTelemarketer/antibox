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
	Vector2_I coords;
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
		p->coords.x = x; p->coords.y = y;
		CheckBounds(p);
		//if (CurrentChunk().localCoords[y][x] == 2) { p->coveredIn = 1; Math::PushBackLog(actionLog, "You are now wet."); }
		PlaceEntities(*p);
	}

	//check if player moves to the next chunk
	void CheckBounds(Player* p) { 
		bool changed = false;
		if (p->coords.x > CHUNK_WIDTH - 1) {
			changed = true;
			p->coords.x = 0;
			c_glCoords.x++;
		}
		else if (p->coords.x < 0) {
			changed = true;
			p->coords.x = CHUNK_WIDTH - 1;
			c_glCoords.x--;
		}
		else if (p->coords.y > CHUNK_HEIGHT - 1) {
			changed = true;
			p->coords.y = 0;
			c_glCoords.y++;
		}
		else if (p->coords.y < 0) {
			changed = true;
			p->coords.y = CHUNK_HEIGHT - 1;
			c_glCoords.y--;
		}
		if (changed) {
			SetShownChunk();
		}
	}
	//check if entity moves to next chunk
	void CheckBounds(Entity* p) { 
		bool changed = false;
		int oldIndex = p->index;
		if (p->coords.x > CHUNK_WIDTH - 1) {
			changed = true;
			p->coords.x = 0;
			gameMap.map[c_glCoords.x + 1][c_glCoords.y].entities.push_back(p);
			p->index = gameMap.map[c_glCoords.x + 1][c_glCoords.y].entities.size() - 1;
		}
		else if (p->coords.x < 0) {
			changed = true;
			p->coords.x = CHUNK_WIDTH - 1;
			gameMap.map[c_glCoords.x - 1][c_glCoords.y].entities.push_back(p);
			p->index = gameMap.map[c_glCoords.x - 1][c_glCoords.y].entities.size() - 1;
		}
		else if (p->coords.y > CHUNK_HEIGHT - 1) {
			changed = true;
			p->coords.y = 0;
			gameMap.map[c_glCoords.x][c_glCoords.y + 1].entities.push_back(p);
			p->index = gameMap.map[c_glCoords.x][c_glCoords.y + 1].entities.size() - 1;
		}
		else if (p->coords.y < 0) {
			changed = true;
			p->coords.y = CHUNK_HEIGHT - 1;
			gameMap.map[c_glCoords.x][c_glCoords.y - 1].entities.push_back(p);
			p->index = gameMap.map[c_glCoords.x][c_glCoords.y - 1].entities.size() - 1;
		}
		if (changed) {
			CurrentChunk().entities.erase(CurrentChunk().entities.begin() + oldIndex);
			FixEntityIndex();
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
		if (entityLayer.localCoords[p.coords.y + 1][p.coords.x] == '#' ||
			entityLayer.localCoords[p.coords.y - 1][p.coords.x] == '#' ||
			entityLayer.localCoords[p.coords.y][p.coords.x + 1] == '#' ||
			entityLayer.localCoords[p.coords.y][p.coords.x - 1] == '#') {
			return true;
		}
		return false;
	}

	void ClearEntities(Player p)
	{
		//go to the player and all entities and replace the original tile
		entityLayer.localCoords[p.coords.y][p.coords.x] = CurrentChunk().localCoords[p.coords.y][p.coords.x];
		for (int i = 0; i < CurrentChunk().entities.size(); i++)
		{
			Entity& curEn = *CurrentChunk().entities[i];
			entityLayer.localCoords[curEn.coords.y][curEn.coords.x] = CurrentChunk().localCoords[curEn.coords.y][curEn.coords.x];
		}
	}

	void PlaceEntities(Player p)
	{
		//place the player and all entities on top of the tiles
		entityLayer.localCoords[p.coords.y][p.coords.x] = 0;
		int id;
		for (int i = 0; i < CurrentChunk().entities.size(); i++)
		{
			if (CurrentChunk().entities[i]->name == "Zombie") { id = 36; }
			else if (CurrentChunk().entities[i]->name == "Chicken") { id = 37; }
			else { id = 35; }
			entityLayer.localCoords[CurrentChunk().entities[i]->coords.y][CurrentChunk().entities[i]->coords.x] = id;
		}
	}

	//Reset the indices of the entities
	void FixEntityIndex() {
		for (int i = 0; i < CurrentChunk().entities.size(); i++)
		{
			CurrentChunk().entities[i]->index = i;
		}
	}

};
