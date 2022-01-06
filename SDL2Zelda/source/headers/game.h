#ifndef GAME_H
#define GAME_H

#include "gamemap.h"
#include "player.h"

class Game
{
public:
	Game();
	~Game();

private:
		
	bool gameRunning;
	Graphics graphics;
	GameMap* gm;
	Player* p;


	void initializeSDL();
	void gameLoop();

	std::vector<SDL_Event>& getFrameEvents();
	void handleInput();
	

	void gameUpdate(Uint32 elapsed);
	void gameDraw();

};

#endif