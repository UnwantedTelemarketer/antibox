#pragma once
#include "entities.h"

#define ITEM_CONTAINER 1
#define ITEM_BANDAGE 2
#define ITEM_GRASS 3

Item canteen = 
{   "Canteen (Empty)",
	ITEM_CONTAINER,
	false,
	true,
	false,
	ActionEffect{quench, quench},
	1,
	"You drink from the canteen.",
	"You pour the canteen on yourself." 
};

Item bandage = 
{   "Sterile Bandage",
	ITEM_BANDAGE,
	true,
	false,
	true,
	{none, heal},
	5,
	"You choke down a bandage.",
	"You wrap a bandage on yourself." 
};

Item grass =
{   "Grass",
	ITEM_GRASS,
	true,
	false,
	true,
	{saturate, none},
	1,
	"You eat the grass.",
	"You wrap grass around yourself."
};

Tile Tile_Grass =
{
	ID_GRASS,
	nothing,
	nullptr,
	false,
	ID_GRASS
};

Tile Tile_TallGrass =
{
	ID_FLOWER,
	nothing,
	nullptr,
	true,
	ID_GRASS
};

Tile Tile_Puddle =
{
	ID_DIRT,
	water,
	nullptr,
	true,
	ID_DIRT
};