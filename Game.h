#pragma once
#include "antibox/core/antibox.h"
#include "antibox/managers/factory.h"

struct Player {
	float health = 0;
	std::string name = "Blank";
	int xCoord = 0;
	int yCoord = 0;
	int coveredIn = 0; // 0 = nothing, 1 = water, 2 = mud
};

struct Chunk {
	int coords[20][30];
};

struct NPC {
	float health;
	float damage;
	std::string name;
};

class Map {
public:
	Chunk testChunk;

	void MovePlayer(int x, int y, Player* p, std::vector<std::string>* actionLog) {
		p->xCoord = x; p->yCoord = y;
		if (testChunk.coords[y][x] == 2) { p->coveredIn = 1; Math::PushBackLog(actionLog, "You are now wet."); }
		BuildMap(*p); 
	}
	bool NearNPC(Player p) {
		//check around player
		if (testChunk.coords[p.yCoord + 1][p.xCoord] == '#' ||
			testChunk.coords[p.yCoord - 1][p.xCoord]  == '#' ||
			testChunk.coords[p.yCoord][p.xCoord + 1]  == '#' ||
			testChunk.coords[p.yCoord][p.xCoord - 1]  == '#') {
			return true;
		}
		return false;
	}

	void BuildMap(Player p) {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 30; j++) {
				if(i == p.yCoord && j == p.xCoord){ testChunk.coords[i][j] = 0; }
				else if (i < 7 && j < 7) { testChunk.coords[i][j] = 2; }
				else if (i == 12 && j == 7) { testChunk.coords[i][j] = 35; }
				else { testChunk.coords[i][j] = 1; }
			}
		}
	}
};

class GameManager {
private:
	std::vector<std::string> missMessages = { "blank", "You swing at nothing and almost fall over.", "You miss.", "You don't hit anything." };
public:
	Map mainMap;
	Player mainPlayer;
	std::vector<std::string> actionLog;
	std::vector<std::string> possibleNames = { "John", "Zombie", "Goblin", "Rat", "Skeleton" };

	void Setup(int x, int y) {
		mainPlayer.xCoord = x;
		mainPlayer.yCoord = y;
		mainMap.BuildMap(mainPlayer);
	}

	void AttemptAttack() {
		if (mainMap.NearNPC(mainPlayer)) {
			if (Math::RandNum(10) <= 3) { mainPlayer.health -= 5; }

		}
		else {
			Math::PushBackLog(&actionLog, Math::RandString(missMessages));
		}
	}

	void MovePlayer(int dir) {
		switch (dir) {
		case 1:
			mainMap.MovePlayer(mainPlayer.xCoord, mainPlayer.yCoord - 1, &mainPlayer, &actionLog);
			break;
		case 2:
			mainMap.MovePlayer(mainPlayer.xCoord, mainPlayer.yCoord + 1, &mainPlayer, &actionLog);
			break;
		case 3:
			mainMap.MovePlayer(mainPlayer.xCoord - 1, mainPlayer.yCoord, &mainPlayer, &actionLog);
			break;
		case 4:
			mainMap.MovePlayer(mainPlayer.xCoord + 1, mainPlayer.yCoord, &mainPlayer, &actionLog);
			break;
		default:
			break;
		}
		if (mainMap.NearNPC(mainPlayer)) { Math::PushBackLog(&actionLog, "Howdy!"); }
	}

	bool NearNPC() {
		return mainMap.NearNPC(mainPlayer);
	}

	void SpawnEnemy(NPC curNPC) {
		curNPC.health = Math::RandNum(100);
		curNPC.damage = Math::RandNum(25);
		curNPC.name = Math::RandString(possibleNames);
	}
};