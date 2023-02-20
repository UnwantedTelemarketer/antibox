#pragma once
#include "antibox/core/mathlib.h"
#include "json/json.h"
#include <fstream>
using json = nlohmann::json;


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
		else if (tile->collectible) 
		{
			AddItem(grass);
			return true;
		}
		return false;
	}

	bool AttemptAction(Action act, Item* item, Player* p)
	{
		if (item->count <= 0) { return false; }

		switch (act) {
		case use:
			switch (item->use.onBodyUse)
			{
			case heal:
				p->health += 5.f;
				break;
			case quench:
				if (item->holdsLiquid && item->liquidAmount > 0.f)
				{
					p->coveredIn = item->coveredIn;
					item->liquidAmount -= 25.f;
				}
				else
				{
					return false;
				}
				p->thirst += 1.5f;
				break;
			case saturate:
				p->hunger += 1.5f;
				break;
			case damage:
				p->health -= 5.f;
				break;
			default:
				return false;
			}
			return true;
			break;
		case consume:
			switch (item->use.onConsume)
			{
			case heal:
				p->health += 5.f;
				break;
			case quench:
				if (item->holdsLiquid && item->liquidAmount > 0.f)
				{
					item->liquidAmount -= 25.f;
					p->thirst += 1.5f;
					p->thirst = std::min(100.f, p->thirst);
				}
				else 
				{
					return false;
				}
				break;
			case saturate:
				p->hunger += 1.5f;
				break;
			case damage:
				p->health -= 5.f;
				break;
			default:
				return false;
			}
			return true;
			break;
		}
		return false;
	}

};
