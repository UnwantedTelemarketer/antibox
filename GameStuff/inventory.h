#pragma once
#include "antibox/core/mathlib.h"

class Inventory
{
public:
	Vector3 clothes;
	std::vector<Item> items;

	void AddItem(Item it)
	{
		if (it.stackable) {
			for (int i = 0; i < items.size(); i++)
			{
				if (items[i].id == it.id)
				{
					items[i].count += it.count;
					return;
				}
			}
		}
		items.push_back(it);
	}

	bool TryGetItem(int itemID, bool needEmpty, Item** itemIndex) 
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i].id == itemID)
			{
				if (needEmpty && items[i].coveredIn != nothing) { continue; }
				*itemIndex = &items[i];
				return true;
			}
		}
		return false;
	}

	bool AttemptCollect(Tile* tile) {
		Item* item;
		if (tile->liquid != nothing)
		{
			if (TryGetItem(ITEM_CONTAINER, false, &item)) {
				if (item->liquidAmount < 100.f)
				{
					item->coveredIn = tile->liquid;
					item->liquidAmount += 25.f;
					item->name = item->liquidAmount != 100.f ? "Canteen (Partially Full)" : "Canteen (Full)";
					return true;
				}
			}
		}
		return false;
	}
};
