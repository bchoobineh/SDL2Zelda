#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "tile.h"
#include "rectangle.h"
#include "player.h"
#include "tinyxml2.h"
#include "enemy.h"
#include "octorok.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 11
#define SCENE_TRANSIT_DELAY 40

struct MapChunk
{
	std::vector<Tile> tiles;
};

class GameMap
{
public:
	GameMap(std::string &filePath, Graphics &graphics);
	~GameMap();

	void drawMap(Graphics& graphics);
	void update(Uint32 elapsed, Player *player);

	void moveMapRight();
	void moveMapLeft();
	void moveMapUp();
	void moveMapDown();

	bool checkTransit() const;

	std::vector<Rectangle> getChunkCollisions(const Rectangle& other);


private:
	
	// Map Tiles
	Tile** mapTiles;

	// Tile Tilesets
	std::vector<Tileset> tileSets;

	// Map Transition Collisions
	std::map<int, std::vector<Rectangle>> mapTransitions;

	// Map Collisions Blocks
	std::map<int, std::vector<Rectangle>> mapCollisions;

	// Map Enemies
	std::map<int, std::vector<Enemy*>> mapEnemies;

	// Location window is anchored on
	Vector2 windowAnchor;

	// Scene Transition Timer
	Uint32 sceneTransitTimer;

	// Location of transit destination
	Vector2 transitTarget;
	Vector2 transitDir;

	// Map Dimensions
	int mapWidth;
	int mapHeight;

	// Current active chunk
	int currentChunk;

	// Chunk Change Animation
	bool inTransit;

	void parseMapData(std::string& filePath, Graphics &graphics);

};

#endif