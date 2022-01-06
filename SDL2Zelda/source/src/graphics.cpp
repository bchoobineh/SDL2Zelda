#include "graphics.h"

Graphics::Graphics()
{
	// Attempt to create SDL Window and Renderer
	if (SDL_CreateWindowAndRenderer(Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 0, &window, &renderer) != 0)
	{
		printf("SDL Window and Renderer creation failed!...\n");
		exit(1);
	}
	printf("SDL Window and Renderer initialized!...\n");
}

Graphics::~Graphics()
{
	// Destroy Window and Renderer
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void Graphics::drawScreen()
{
	SDL_RenderPresent(renderer);
}

void Graphics::clearScreen()
{
	SDL_RenderClear(renderer);
}

void Graphics::drawTexture(SDL_Texture* tex, SDL_Rect* src, SDL_Rect* dst)
{
	SDL_RenderCopy(renderer, tex, src, dst);
}

SDL_Surface* Graphics::loadSpriteSheet(std::string& filePath)
{
	// If Sprite Sheet is not loaded yet, load it in
	if (spriteSheets.count(filePath) == 0)
		spriteSheets[filePath] = IMG_Load(filePath.c_str());

	return spriteSheets[filePath];
}

SDL_Renderer* Graphics::getRenderer() const
{
	return renderer;
}