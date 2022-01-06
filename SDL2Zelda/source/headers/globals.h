#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

namespace Global
{
	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 896;
	const int SPRITE_SCALE = 4;
	const int FPS = 60;
	const Uint32 FRAME_TIME_MAX = 1000 / FPS;

	enum class CollisionSide
	{
		TOP,
		BOT,
		LEFT,
		RIGHT,
		NONE
	};

	enum class Facing
	{
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

}

struct Vector2
{
	int x, y;
	Vector2() :
		x(0), y(0)
	{}

	Vector2(int x, int y) :
		x(x), y(y)
	{}
};

struct Tileset
{
	SDL_Texture* tex;
	Vector2 tileSize;
	int firstGid;

	Tileset() :
		tex(nullptr), firstGid(-1), tileSize(Vector2(0,0))
	{}

	Tileset(SDL_Texture* tex, int firstGid, Vector2 tileSize) :
		tex(tex), firstGid(firstGid), tileSize(tileSize)
	{}
};


#endif