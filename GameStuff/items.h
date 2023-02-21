#pragma once
#include "entities.h"

#define ITEM_CONTAINER 1
#define ITEM_BANDAGE 2
#define ITEM_GRASS 3

Item canteen = 
{   "Canteen",							   //Name
	ITEM_CONTAINER,						   //ID
	false,								   //Stackable
	true,								   //Holds Liquid
	false,								   //Consumable
	ActionEffect{{quench, 5.f}, {quench}}, //OnConsume and OnBodyUse
	1,									   //Amount
	"You drink from the canteen.",		   //OnConsume message
	"You pour the canteen on yourself."	   //OnBodyUse message
}; 

Item jar =
{	"Mason Jar",
	ITEM_CONTAINER,
	false,
	true,
	false,
	{{quench, 4.f}, {quench}},
	1,
	"You drink from the mason jar.",
	"You pour out the contents of the mason jar on yourself."
};

Item bandage = 
{   "Sterile Bandage",
	ITEM_BANDAGE,
	true,
	false,
	true,
	{{none}, {heal, 5.f}},
	5,
	"You eat bandage.",
	"You wrap a bandage around your wound." 
};

Item grass =
{   "Grass",
	ITEM_GRASS,
	true,
	false,
	true,
	{{saturate, 1.f}, {heal, 1.f}},
	1,
	"You eat the grass.",
	"You wrap grass around your wound."
};

Tile Tile_Grass =
{
	ID_GRASS, //Starting Block
	nothing,  //Liquid
	nullptr,  //Entity
	false,	  //Collectible
	ID_GRASS  //Block after collection
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