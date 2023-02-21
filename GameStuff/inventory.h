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
				if (items[i].coveredIn != nothing && items[i].liquidAmount >= 100.f) { continue; }
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
		float amount;
		switch (act) {										//Check if we are consuming or using
		case use:
			amount = item->use.onBodyUse.amount;
			switch (item->use.onBodyUse.effect)				//If we are using, check what effect this item has on use
			{
			case heal:
				p->health += amount;
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
				p->thirst += amount;
				break;
			case saturate:
				p->hunger += amount;
				break;
			case damage:
				p->health -= amount;
				break;
			default:
				return false;
			}    
			return true;
			break;
		case consume:
			amount = item->use.onConsume.amount;
			switch (item->use.onConsume.effect)				//If we are consuming, check what effect this item has on consume
			{
			case heal:
				p->health += amount;
				break;
			case quench:
				if (item->holdsLiquid && item->liquidAmount > 0.f)
				{
					item->liquidAmount -= 25.f;
					p->thirst += amount;
					p->thirst = std::min(100.f, p->thirst);
				}
				else 
				{
					return false;
				}
				break;
			case saturate:
				p->hunger += amount;
				break;
			case damage:
				p->health -= amount;
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
