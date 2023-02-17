#pragma once



struct Entity {
	float health;
	const char* name;
	int xCoord, yCoord;
};

#define TILE_WATER "~"
#define TILE_GRASS "."
#define TILE_TREE "^"
#define TILE_FLOWER "*"
#define ENT_ZOMBIE "Z"
#define ENT_PLAYER "@"
#define ENT_HUMAN "&"