#include "sprite.h"

Sprite::Sprite()
{
	sourceRect = { 0,0,0,0 };
	spriteSheet = nullptr;
	xPos = 0.0f;
	yPos = 0.0f;
}

Sprite::Sprite(Graphics& graphics, std::string& filePath, int srcX, int srcY, int width, int height, float xPos, float yPos)
{
	sourceRect = { srcX, srcY, width, height };

	spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadSpriteSheet(filePath));
	if (spriteSheet == NULL)
	{
		printf("Unable to load image: %s\n", filePath.c_str());
		exit(1);
	}
	this->xPos = xPos;
	this->yPos = yPos;
	boundingBox = Rectangle(xPos, yPos, float(width * Global::SPRITE_SCALE), float(height * Global::SPRITE_SCALE));
}

Sprite::~Sprite()
{}

void Sprite::update()
{
	// Update sprite's bounding box
	boundingBox = Rectangle(xPos, yPos, float(sourceRect.w * Global::SPRITE_SCALE), float(sourceRect.h * Global::SPRITE_SCALE));
}

void Sprite::draw(Graphics& graphics, SDL_Rect dst)
{
	graphics.drawTexture(spriteSheet, &sourceRect, &dst);
}

Rectangle Sprite::getBoundingBox() const
{
	return boundingBox;
}

float Sprite::getXPosition() const
{
	return xPos;
}

float Sprite::getYPosition() const
{
	return yPos;
}

