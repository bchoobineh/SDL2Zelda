#include "game.h"

Game::Game()
{
	// Initialize SDL
	initializeSDL();

	// Game is now running
	gameRunning = true;

	// Launch the Game Loop
	gameLoop();	
}

Game::~Game()
{
	// Clean up SDL
	SDL_Quit();
	printf("SDL Cleaned Up!...\n");

	// TEMP
	delete(gm);
	delete(p);
}

void Game::initializeSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		printf("SDL Initialized Successfully!...\n");
	}
	else
	{
		printf("SDL Initialization Failed!\n");
		exit(1);
	}
}

void Game::gameLoop()
{
	std::string test = "content/maps/overworldMap.tmx";
	std::string test2 = "content/spriteSheets/linkCard.png";

	gm = new GameMap(test, graphics);
	p = new Player(graphics, test2);

	Uint32 lastUpdate = SDL_GetTicks();
	while (gameRunning)
	{
		handleInput();

		Uint32 currentTime = SDL_GetTicks();
		Uint32 elapsed = currentTime - lastUpdate;
		if (elapsed < Global::FRAME_TIME_MAX)
			gameUpdate(elapsed);
		else
			gameUpdate(Global::FRAME_TIME_MAX);

		lastUpdate = currentTime;



		gameDraw();
	}
}

std::vector<SDL_Event>& Game::getFrameEvents()
{
	static std::vector<SDL_Event> frameEvents;
	return frameEvents;
}

void Game::handleInput()
{
	// Gather frames inputs
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		// If user ever specifies to end program
		if (event.type == SDL_QUIT)
			gameRunning = false;

		// Queue up events
		getFrameEvents().push_back(event);
	}

	std::vector<SDL_Event> events = getFrameEvents();

	// Update player when applicable
	if(!gm->checkTransit())
		p->handleInput(events);

	// Clear Event Queue
	getFrameEvents().clear();
}

void Game::gameUpdate(Uint32 elapsed)
{
	gm->update(elapsed, p);
	p->update(elapsed);

	// Check Collisions
	std::vector<Rectangle> tmp;
	if ((tmp = gm->getChunkCollisions(p->getBoundingBox())).size() > 0)
	{
		p->processTileCollisions(tmp);
	}

	
}

void Game::gameDraw()
{
	graphics.clearScreen();

	gm->drawMap(graphics);

	if(!gm->checkTransit())
		p->draw(graphics);

	graphics.drawScreen();
}