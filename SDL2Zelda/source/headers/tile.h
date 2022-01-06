#ifndef TILE_H
#define TILE_H

#include "globals.h"
#include "graphics.h"

class Tile
{
public:
	Tile();
	Tile(Tileset spriteSheet, Vector2 sheetPosition);
	~Tile();

	void draw(Graphics& graphics, SDL_Rect* dst);	

private:
	Tileset sourceSheet;
	Vector2 textureLocation;

};

#endif