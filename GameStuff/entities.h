#pragma once

enum Behaviour {Wander, Slow_Wander, Stationary};
enum Liquid {nothing, water, mud, fire};

struct Entity {
	float health;
	const char* name;
	int entityID;
	Behaviour b;
	bool aggressive;

	Vector2_I coords;
	int index; //in entity list

};

struct Tile {
	int id;
	Liquid liquid;
	Entity* entity;
};

struct Item {
	std::string name;
	int id;
	bool stackable, holdsLiquid;
	int count = 1;

	float weight;
	Liquid coveredIn = nothing; //liquids
	float liquidAmount = 0.f;
};

#define TILE_LIQUID "~"

#define TILE_NULL "?"
#define ID_NULL -1

#define TILE_GRASS "."
#define ID_GRASS 1

#define TILE_DIRT ","
#define ID_DIRT 2

#define TILE_TREE "^"
#define ID_TREE 999

#define TILE_FLOWER "#"
#define ID_FLOWER 3

#define TILE_FIRE "&"
#define ID_FIRE 4

#define ENT_PLAYER "@"
#define ID_PLAYER 0

#define ENT_ZOMBIE "Z"
#define ID_ZOMBIE 1

#define ENT_CHICKEN "C"
#define ID_CHICKEN 2

#define ENT_HUMAN "&"
#define ID_HUMAN 3

#define ITEM_CONTAINER 1