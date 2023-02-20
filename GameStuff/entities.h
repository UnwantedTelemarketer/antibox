#pragma once

enum Behaviour {Wander, Slow_Wander, Stationary};

struct Entity {
	float health;
	const char* name;
	Behaviour b;
	bool aggressive;

	Vector2_I coords;
	int index; //in entity list

};

struct Item {
	std::string name;
	int id;
	int count;

	float weight;
	int coveredIn; //liquids
	bool container;//holds liquid
};

#define TILE_WATER "~"
#define TILE_GRASS "."
#define TILE_TREE "^"
#define TILE_FLOWER "#"
#define ENT_ZOMBIE "Z"
#define ENT_CHICKEN "C"
#define ENT_PLAYER "@"
#define ENT_HUMAN "&"