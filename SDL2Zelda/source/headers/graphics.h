#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "globals.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	void drawScreen();
	void clearScreen();

	void drawTexture(SDL_Texture* tex, SDL_Rect* src, SDL_Rect* dst);

	SDL_Surface* loadSpriteSheet(std::string& filePath);

	SDL_Renderer* getRenderer() const;

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	std::map<std::string, SDL_Surface*> spriteSheets;
	
};

#endif