#pragma once
#include "antibox/core/antibox.h"
#include "antibox/managers/factory.h"

#include "items.h"
#include "inventory.h"
#include <algorithm>
#include <cmath>
#include <thread>

#define MAP_UP 1
#define MAP_DOWN 2
#define MAP_LEFT 3
#define MAP_RIGHT 4
#define CHUNK_WIDTH 30
#define CHUNK_HEIGHT 30
#define MAP_HEIGHT 10
#define MAP_WIDTH 10

struct Chunk {
	Vector2_I globalChunkCoord;
	Tile localCoords[CHUNK_WIDTH][CHUNK_HEIGHT];
	std::vector<Entity*> entities;
};
struct T_Chunk //we dont need the current chunk copy to have an entity list
{
	int localCoords[CHUNK_WIDTH][CHUNK_HEIGHT];
};

struct World
{
	Chunk chunks[MAP_WIDTH][MAP_HEIGHT];
};


class Map {
public:
	Vector2_I c_glCoords{ 5, 5 };
	T_Chunk effectLayer; //is the original chunk with entities placed over it
	World world; //keeps the original generated map so that tiles walked over wont be erased
	std::vector<Vector2_I> line;

	void CreateMap()
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				world.chunks[x][y].globalChunkCoord.x = x;
				world.chunks[x][y].globalChunkCoord.y = y;
				BuildChunk(&world.chunks[x][y]);
				SpawnChunkEntities(&world.chunks[x][y]);
			}
		}
	}

	void SpawnChunkEntities(Chunk* chunk) 
	{
		for (int i = 0; i < Math::RandInt(10); i++)
		{
			Entity* zomb = new Entity{ 25, "Zombie", ID_ZOMBIE, Wander, true, Math::RandInt(CHUNK_WIDTH), Math::RandInt(CHUNK_HEIGHT) };
			chunk->entities.push_back(zomb);
		}
	}

	Chunk& CurrentChunk() {
		return world.chunks[c_glCoords.x][c_glCoords.y];
	}

	Tile* TileAtPos(Vector2_I coords)
	{
		return &world.chunks[c_glCoords.x][c_glCoords.y].localCoords[coords.x][coords.y];
	}

	//T_Chunk& EntityLayer() { return entityLayer; }

	void MovePlayer(int x, int y, Player* p, std::vector<std::string>* actionLog) {
		p->coords.x = x; p->coords.y = y;
		CheckBounds(p);
		//if (CurrentChunk().localCoords[y][x] == 2) { p->coveredIn = 1; Math::PushBackLog(actionLog, "You are now wet."); }
	}

	//check if player moves to the next chunk
	void CheckBounds(Player* p) { 
		bool changed = false;
		if (p->coords.x > CHUNK_WIDTH - 1) {
			c_glCoords.x++;
			if (c_glCoords.x >= CHUNK_WIDTH - 1) { c_glCoords.x = CHUNK_WIDTH - 1; goto offscreen; }
			changed = true;
			p->coords.x = 0;
		}
		else if (p->coords.x < 0) {
			c_glCoords.x--;
			if (c_glCoords.x < 0) { c_glCoords.x = 0; goto offscreen; }
			changed = true;
			p->coords.x = CHUNK_WIDTH - 1;
		}
		else if (p->coords.y > CHUNK_HEIGHT - 1) {
			c_glCoords.y++;
			if (c_glCoords.y >= CHUNK_WIDTH - 1) 
			{ 
				c_glCoords.y = CHUNK_WIDTH - 1; 
				goto offscreen; 
			}
			changed = true;
			p->coords.y = 0;
		}
		else if (p->coords.y < 0) {
			c_glCoords.y--;
			if (c_glCoords.y < 0) { c_glCoords.y = 0; goto offscreen; }
			changed = true;
			p->coords.y = CHUNK_HEIGHT - 1;
		}

		//if the player would go offscreen, reset them
		//Evil goto statement >:)
		offscreen:
		if (p->coords.x >= CHUNK_WIDTH) { p->coords.x = CHUNK_WIDTH - 1; }
		else if (p->coords.y >= CHUNK_HEIGHT) { p->coords.y = CHUNK_HEIGHT - 1; }
		else if (p->coords.x < 0) { p->coords.x = 0; }
		else if (p->coords.y < 0) { p->coords.y = 0; }
	}
	//check if entity moves to next chunk
	void CheckBounds(Entity* p, Chunk* chunk) { 
		bool changed = false;
		int oldIndex = p->index;
		if (p->coords.x > CHUNK_WIDTH - 1) {
			if (c_glCoords.x + 1 >= CHUNK_WIDTH) { return; }
			changed = true;
			p->coords.x = 0;
			world.chunks[c_glCoords.x + 1][c_glCoords.y].entities.push_back(p);
			p->index = world.chunks[c_glCoords.x + 1][c_glCoords.y].entities.size() - 1;
		}
		else if (p->coords.x < 0) {
			if (c_glCoords.x - 1 < 0) { return; }
			changed = true;
			p->coords.x = CHUNK_WIDTH - 1;
			world.chunks[c_glCoords.x - 1][c_glCoords.y].entities.push_back(p);
			p->index = world.chunks[c_glCoords.x - 1][c_glCoords.y].entities.size() - 1;
		}
		else if (p->coords.y > CHUNK_HEIGHT - 1) {
			if (c_glCoords.y + 1 >= CHUNK_HEIGHT) { return; }
			changed = true;
			p->coords.y = 0;
			world.chunks[c_glCoords.x][c_glCoords.y + 1].entities.push_back(p);
			p->index = world.chunks[c_glCoords.x][c_glCoords.y + 1].entities.size() - 1;
		}
		else if (p->coords.y < 0) {
			if (c_glCoords.y - 1 < 0) { return; }
			changed = true;
			p->coords.y = CHUNK_HEIGHT - 1;
			world.chunks[c_glCoords.x][c_glCoords.y - 1].entities.push_back(p);
			p->index = world.chunks[c_glCoords.x][c_glCoords.y - 1].entities.size() - 1;
		}
		if (changed) {
			chunk->entities.erase(chunk->entities.begin() + oldIndex);
			FixEntityIndex(chunk);
		}
	}

	void CreateStarterChunk(Player p) {
		//SetShownChunk();
	}

	void BuildChunk(Chunk* chunk) {
		for (int i = 0; i < CHUNK_WIDTH; i++) {
			for (int j = 0; j < CHUNK_HEIGHT; j++) {
				int currentTile = Math::RandNum(10);
				if (currentTile > 9) { chunk->localCoords[i][j] = Tile_Puddle; }
				else if (currentTile > 8) { chunk->localCoords[i][j] = Tile_TallGrass; }
				else { chunk->localCoords[i][j] = Tile_Grass; }
			}
		}
	}

	std::vector<Vector2_I> GetLine(Vector2_I startTile, Vector2_I endTile, int limit) {
		std::vector<Vector2_I> lineList;
		int x = startTile.x;
		int y = startTile.y;
		int w = endTile.x - x;
		int h = endTile.y - y;
		int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
		if (w < 0) dx1 = -1; else if (w > 0) dx1 = 1;
		if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
		if (w < 0) dx2 = -1; else if (w > 0) dx2 = 1;
		int longest = abs(w);
		int shortest = abs(h);
		if (!(longest > shortest))
		{
			longest = abs(h);
			shortest = abs(w);
			if (h < 0) dy2 = -1; else if (h > 0) dy2 = 1;
			dx2 = 0;
		}
		int numerator = longest >> 1;
		int limited = std::min(longest, limit);
		for (int i = 0; i <= limited; i++)
		{
			//if (GetTileAtPos(new Vector2(x, y)) == null) return tilesLine;

			lineList.push_back({x, y});

			numerator += shortest;
			if (!(numerator < longest))
			{
				numerator -= longest;
				x += dx1;
				y += dy1;
			}
			else
			{
				x += dx2;
				y += dy2;
			}
		}
		return lineList;
	}

	//Start tile, End tile
	void DrawLine(Vector2_I sTile, Vector2_I eTile) 
	{
		ClearLine();
		line = GetLine(sTile, eTile, 15);

		for (int i = 0; i < line.size(); i++)
		{
			effectLayer.localCoords[line[i].x][line[i].y] = 15;
		}
	}

	void ClearLine()
	{
		for (int i = 0; i < line.size(); i++)
		{
			effectLayer.localCoords[line[i].x][line[i].y] = 0;
		}
		line.clear();
	}

	void ClearEntities(Chunk* chunk)
	{
		//go to the player and all entities and replace the original tile
		for (int i = 0; i < chunk->entities.size(); i++)
		{
			Entity& curEn = *chunk->entities[i];
			chunk->localCoords[curEn.coords.y][curEn.coords.x].entity = nullptr;
		}
	}

	void PlaceEntities(Chunk* chunk)
	{
		int id;
		for (int i = 0; i < chunk->entities.size(); i++)
		{
			chunk->localCoords
				[chunk->entities[i]->coords.y]
			[chunk->entities[i]->coords.x].entity = chunk->entities[i];
		}

	}

	//Reset the indices of the entities
	void FixEntityIndex(Chunk* chunk) {
		for (int i = 0; i < chunk->entities.size(); i++)
		{
			chunk->entities[i]->index = i;
		}
	}


	~Map() 
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				for (int i = 0; i < world.chunks[x][y].entities.size(); i++)
				{
					delete(&world.chunks[x][y].entities[i]);
				}
			}
		}
	}

};
