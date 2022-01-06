#ifndef SPRITE_H
#define SPRITE_H

#include "graphics.h"
#include "globals.h"
#include "rectangle.h"

class Sprite
{
public:
	Sprite();
	Sprite(Graphics& graphics, std::string& filePath, int srcX, int srcY, int width, int height, float xPos, float yPos);
	~Sprite();

	virtual void update();
	void draw(Graphics& graphics, SDL_Rect dst);

	Rectangle getBoundingBox() const;

	float getXPosition() const;
	float getYPosition() const;



protected:
	SDL_Rect sourceRect;
	SDL_Texture* spriteSheet;

	Rectangle boundingBox;

	float xPos, yPos;

};

#endif