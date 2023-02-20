#pragma once
#include "antibox/core/mathlib.h"

class Inventory
{
public:
	Vector3 clothes;
	std::vector<Item> items;

	void AddItem(Item it)
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i].id == it.id) 
			{
				items[i].count += it.count;
				return;
			}
		}
		items.push_back(it);
	}
};
