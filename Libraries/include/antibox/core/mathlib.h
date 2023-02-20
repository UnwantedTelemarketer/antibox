#pragma once
#include <random>



namespace Math{
	//Input the max number, and this will return a float between 1 and your max (inclusive).
	inline float RandNum(int max) { return (rand() % max + 1); } //kinda gross to look at, but its just getting a random seed and returning the float

	//Input the max number, and this will return an int between 1 and your max (inclusive).
	inline int RandInt(int max) { return (rand() % max + 1); } //kinda gross to look at, but its just getting a random seed and returning the float

	//Returns a random string from a vector of strings.
	inline std::string RandString(std::vector<std::string> list) { return list[rand() % (list.size()-1) + 1]; } //also bad to look at, but i dont want to make a multiline function in a header

	//Limits the size of a vector of strings to 15, if anything goes over that amount, the oldest element is deleted and the new one is added.
	inline void PushBackLog(std::vector<std::string>* log, std::string message){
		if (log->size() <= 15) { log->push_back(message); }
		else { log->erase(log->begin()); log->push_back(message); }
	}
}

struct Vector2_I
{
	int x, y;

	bool operator==(Vector2_I vec2) {
		if (vec2.x == x && vec2.y == y) { return true; }
		return false;
	}

	bool operator!=(Vector2_I vec2) {
		if (vec2.x == x && vec2.y == y) { return false; }
		return true;
	}
};

struct Vector2
{
	float x, y;
};

struct Vector3
{
	float x, y, z;
};

struct Vector4
{
	float x, y, z, w;
};

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;