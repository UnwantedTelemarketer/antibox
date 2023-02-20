#pragma once

enum Action { use, consume, combine };
enum Behaviour { Wander, Slow_Wander, Stationary };
enum ConsumeEffect { none, heal, quench, saturate, damage, coverInLiquid };
enum Liquid { nothing, water, mud, fire };

//Health, Name, ID, Behaviour, Aggressive
struct Entity {
	float health;
	const char* name;
	int entityID;
	Behaviour b;
	bool aggressive;

	Vector2_I coords;
	int index; //in entity list

};

//Consume, BodyUse
struct ActionEffect {
	ConsumeEffect onConsume;
	ConsumeEffect onBodyUse;
};

struct Player {
	float health = 100;
	float thirst = 100;
	float hunger = 100;
	std::string name = "Blank";
	Vector2_I coords;
	Liquid coveredIn = nothing;
	int ticksCovered = 0;
};

struct Tile {
	int id;
	Liquid liquid;
	Entity* entity;
	bool collectible;
	int replacementID = id;
};

//Name, ID, Stackable, Holds Liquid, Count, Weight, Liquid, LAmount
struct Item {
	std::string name;
	int id;
	bool stackable, holdsLiquid, consumable;
	ActionEffect use;
	int count = 1;
	std::string consumeTxt, useTxt;

	float weight;
	Liquid coveredIn = nothing; //liquids
	float liquidAmount = 0.f;
};

#define TILE_LIQUID "~"

#define TILE_NULL "?"
#define ID_NULL -1

#define TILE_GRASS "."
#define ID_GRASS 1

#define TILE_DIRT "."
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
#define ITEM_BANDAGE 2
#define ITEM_GRASS 3