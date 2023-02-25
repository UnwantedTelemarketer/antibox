#pragma once
#include <random>

#define maybe (rand() % 10 + 1) >= 5

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

	bool operator==(Vector2 vec2) {
		if (vec2.x == x && vec2.y == y) { return true; }
		return false;
	}

	bool operator!=(Vector2 vec2) {
		if (vec2.x == x && vec2.y == y) { return false; }
		return true;
	}
};

struct Vector3
{
	float x, y, z;

	bool operator==(Vector3 vec3) {
		if (vec3.x == x && vec3.y == y && vec3.z == z) { return true; }
		return false;
	}

	bool operator!=(Vector3 vec3) {
		if (vec3.x == x && vec3.y == y && vec3.z == z) { return false; }
		return true;
	}
};

struct Vector4
{
	float x, y, z, w;
	bool operator==(Vector4 vec4) {
		if (vec4.x == x && vec4.y == y && vec4.z == z && vec4.w == w) { return true; }
		return false;
	}

	bool operator!=(Vector4 vec4) {
		if (vec4.x == x && vec4.y == y && vec4.z == z && vec4.w == w) { return false; }
		return true;
	}
};

struct Vector26
{
	float a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z;
};

typedef Vector2_I vec2_i;
typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vec4;