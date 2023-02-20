#pragma once
#include "entities.h"

Item canteen = 
{   "Canteen (Empty)",
	1,
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
	2,
	true,
	false,
	true,
	{none, heal},
	5,
	"You eat the bandage.",
	"You wrap a bandage on yourself." 
};

Item grass =
{   "Grass",
	3,
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