#include "gamemap.h"

GameMap::GameMap(std::string& filePath, Graphics &graphics)
{
	// No transmit animation active
	inTransit = false;

	// Initialize window anchor location
	windowAnchor = Vector2(0, 0);

	// Get Map data
	parseMapData(filePath, graphics);

	// Transit Direction
	transitTarget = Vector2(0,0);
	transitDir = Vector2(0, 0);

	// Current chunk
	currentChunk = 0;

	// Set transit timer
	sceneTransitTimer = SCENE_TRANSIT_DELAY;
}

GameMap::~GameMap()
{
	// Free Up Map Tiles Array
	for (int i = 0; i < mapHeight; i++)
	{
		delete [] mapTiles[i];
	}
	delete [] mapTiles;
}

void GameMap::parseMapData(std::string& filePath, Graphics &graphics)
{
	// Attempt to load XML Document
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
	{
		printf("Map: %s Loading Failed!\n", filePath.c_str());
		exit(1);
	}

	// Get Dimensions of Map
	tinyxml2::XMLElement* mapNode = doc.FirstChildElement("map");
	mapNode->QueryIntAttribute("width", &mapWidth);
	mapNode->QueryIntAttribute("height", &mapHeight);

	// Allocate Tile Array
	mapTiles = new Tile*[mapHeight];
	for (int i = 0; i < mapHeight; i++)
		mapTiles[i] = new Tile[mapWidth];

	// Parse Tilesets
	tinyxml2::XMLElement* tilesetNode = mapNode->FirstChildElement("tileset");
	while (tilesetNode)
	{
		// Get First GID
		int fGID;
		tilesetNode->QueryIntAttribute("firstgid", &fGID);

		// Get the Tileset Texture
		const char* tileSource = tilesetNode->FirstChildElement("image")->Attribute("source");
		std::string src(&tileSource[3]);
		src = "content/" + src;

		SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadSpriteSheet(src));

		// Get Tile Size
		int tileWidth, tileHeight;
		tilesetNode->QueryIntAttribute("tilewidth", &tileWidth);
		tilesetNode->QueryIntAttribute("tileheight", &tileHeight);

		// Create and Push back Tileset
		tileSets.push_back(Tileset(tex, fGID, Vector2(tileWidth, tileHeight)));

		// Consider any other Tilesets
		tilesetNode = tilesetNode->NextSiblingElement("tileset");
	}

	// Parse Layers
	tinyxml2::XMLElement* layerNode = mapNode->FirstChildElement("layer");
	while (layerNode)
	{
		// Parse Data 
		tinyxml2::XMLElement* dataNode = layerNode->FirstChildElement("data");
		while (dataNode)
		{

			// Parse Tile Data
			tinyxml2::XMLElement* tileNode = dataNode->FirstChildElement("tile");
			int tileCounter = 0;
			while (tileNode)
			{
				// No Tile here
				if (tileNode->IntAttribute("gid") == 0)
				{
					tileCounter++;					
					
					// More Tiles
					if (tileNode->NextSiblingElement("tile"))
					{
						tileNode = tileNode->NextSiblingElement("tile");
						continue;
					}
					else
					{
						break;
					}
				}

				// Find Associated Tileset
				int GID = tileNode->IntAttribute("gid");
				Tileset st;
				int nearest = 0;
				for (int i = 0; i < tileSets.size(); i++)
				{
					if (tileSets.at(i).firstGid <= GID)
					{
						if (tileSets.at(i).firstGid > nearest)
						{
							nearest = tileSets.at(i).firstGid;
							st = tileSets.at(i);
						}
					}
				}

				// If we didn't find any valid Tileset
				if (st.firstGid < 0)
				{
					printf("Could not find any valid Tileset for GID: %d!\n", GID);
					tileCounter++;

					// If there are more Tiles to process
					if (tileNode->NextSiblingElement("tile"))
					{
						tileNode = tileNode->NextSiblingElement("tile");
						continue;
					}
					else
					{
						break;
					}
				}

				// Find Spritesheet coordinates for respective GID
				int sheetX = (tileCounter % mapWidth) * st.tileSize.x;
				int sheetY = st.tileSize.y * ((GID - st.firstGid) / mapWidth);

				// Store Tile object
				int tileX = tileCounter / mapWidth;
				int tileY = tileCounter % mapWidth;

				if (tileY >= 0 && tileY < mapWidth && tileX >= 0 && tileX < mapHeight)
					mapTiles[tileX][tileY] = Tile(st, Vector2(sheetX, sheetY));



				// Determine Chunk ID				
				//int currentRow = tileCounter / mapWidth;
				//int currentCol = tileCounter % mapWidth;				
				//int chunkId = ((currentRow / CHUNK_HEIGHT) * (mapWidth / CHUNK_WIDTH)) + (currentCol / CHUNK_WIDTH);
				//mapChunks[chunkId].tiles.push_back(mapTile);

				tileCounter++;
				tileNode = tileNode->NextSiblingElement("tile");
			}

			dataNode = dataNode->NextSiblingElement("data");
		}

		layerNode = layerNode->NextSiblingElement("layer");
	}

	// Parse through the objects
	tinyxml2::XMLElement* objGroupNode = mapNode->FirstChildElement("objectgroup");
	while (objGroupNode)
	{
		const char* name = objGroupNode->Attribute("name");
		std::string objName(name);

		// Parse Screen Transitions
		if (objName == "ScreenTransitions")
		{
			tinyxml2::XMLElement* objNode = objGroupNode->FirstChildElement("object");
			while (objNode)
			{

				// Parse Rectangle Data
				float x, y, width, height;
				x = objNode->FloatAttribute("x");
				y = objNode->FloatAttribute("y");
				width = objNode->FloatAttribute("width") * Global::SPRITE_SCALE;
				height = objNode->FloatAttribute("height") * Global::SPRITE_SCALE;

				// Determine Chunk Location
				int xSec = int(x) / (256);
				int ySec = int(y) / (176);
				int chunkID = xSec + (ySec * ((mapWidth / CHUNK_WIDTH)));

				// Update X/Y to local coordinates
				x = float(fmod(x, 256));
				y = float(fmod(y, 176));

				// Create Collision Rectangle
				Rectangle r(x * Global::SPRITE_SCALE, y * Global::SPRITE_SCALE, width, height);

				// Add to mapping
				mapTransitions[chunkID].push_back(r);
	

				objNode = objNode->NextSiblingElement("object");
			}
		}

		else if (objName == "Collisions")
		{
			tinyxml2::XMLElement* objNode = objGroupNode->FirstChildElement("object");
			while (objNode)
			{
				// Parse Rectangle Data
				float x, y, width, height;
				x = objNode->FloatAttribute("x");
				y = objNode->FloatAttribute("y");
				width = objNode->FloatAttribute("width") * Global::SPRITE_SCALE;
				height = objNode->FloatAttribute("height") * Global::SPRITE_SCALE;

				// Determine Chunk Location
				int xSec = int(x) / (256);
				int ySec = int(y) / (176);
				int chunkID = xSec + (ySec * ((mapWidth / CHUNK_WIDTH)));

				// Update X/Y to local coordinates
				x = float(fmod(x, 256));
				y = float(fmod(y, 176));

				// Create Collision Rectangle
				Rectangle r(x* Global::SPRITE_SCALE, y* Global::SPRITE_SCALE, width, height);

				// Add to collision mapping
				mapCollisions[chunkID].push_back(r);

				objNode = objNode->NextSiblingElement("object");
			}
		}

		else if (objName == "Enemies")
		{
			tinyxml2::XMLElement* objNode = objGroupNode->FirstChildElement("object");
			while (objNode)
			{
				// Parse Rectangle Data
				float x, y, width, height;
				x = objNode->FloatAttribute("x");
				y = objNode->FloatAttribute("y");
				width = objNode->FloatAttribute("width") * Global::SPRITE_SCALE;
				height = objNode->FloatAttribute("height") * Global::SPRITE_SCALE;

				// Determine Chunk Location
				int xSec = int(x) / (256);
				int ySec = int(y) / (176);
				int chunkID = xSec + (ySec * ((mapWidth / CHUNK_WIDTH)));

				// Update X/Y to local coordinates
				x = float(fmod(x, 256));
				y = float(fmod(y, 176));

				std::string name = objNode->Attribute("name");
				if (name == "Octorok")
				{
					std::string tmp = "content/spriteSheets/enemies.png";
					mapEnemies[chunkID].push_back(new Octorok(graphics, tmp, x*Global::SPRITE_SCALE, y*Global::SPRITE_SCALE));
				}

				objNode = objNode->NextSiblingElement("object");
  			}
		}



		objGroupNode = objGroupNode->NextSiblingElement("objectgroup");
	}


}

void GameMap::drawMap(Graphics& graphics)
{
	SDL_Rect dst = { 0, 192, 64, 64 };
	for (int i = 0; i < CHUNK_HEIGHT; i++)
	{
		dst.x = 0;
		for (int j = 0; j < CHUNK_WIDTH; j++)
		{
			mapTiles[i + windowAnchor.x][j + windowAnchor.y].draw(graphics, &dst);
			dst.x += 64;
		}

		dst.y += 64;
	}

	// Draw Enemies
	for (int i = 0; i < mapEnemies.at(currentChunk).size(); i++)
	{
		mapEnemies.at(currentChunk).at(i)->draw(graphics);
	}

}

void GameMap::update(Uint32 elapsed, Player* player)
{
	// Scene transition happening
	if (inTransit)
	{
		// Update timer
		sceneTransitTimer -= elapsed;

		// Adjust Window Anchor Condition
		if (sceneTransitTimer <= 0)
		{
			sceneTransitTimer = SCENE_TRANSIT_DELAY;
			
			// Update Window Anchor with Transit Dir Vector
			windowAnchor.x += transitDir.x;
			windowAnchor.y += transitDir.y;
		
			// Reached destination
			if (windowAnchor.x == transitTarget.x && windowAnchor.y == transitTarget.y)
				inTransit = false;

		}
	}

	else
	{
		// Update Enemies
		for (int i = 0; i < mapEnemies.at(currentChunk).size(); i++)
		{
			mapEnemies.at(currentChunk).at(i)->update(elapsed, mapCollisions[currentChunk]);
			mapEnemies.at(currentChunk).at(i)->checkDeath(player->getBoundingBox());
		}


		// Check Transition Collisions
		std::vector<Rectangle> rects = mapTransitions[currentChunk];
		for (int i = 0; i < rects.size(); i++)
		{
			if (rects.at(i).inCollision(player->getBoundingBox()))
			{
				
				if (rects.at(i).getCollisionSide(player->getBoundingBox()) == Global::CollisionSide::LEFT)
				{
					moveMapRight();
					player->setPosX(10.0f);
				}

				else if (rects.at(i).getCollisionSide(player->getBoundingBox()) == Global::CollisionSide::RIGHT)
				{
					moveMapLeft();
					player->setPosX(950.0f);
				}

				else if (rects.at(i).getCollisionSide(player->getBoundingBox()) == Global::CollisionSide::TOP)
				{
					moveMapDown();
					player->setPosY(10.0f);
				}

				else if (rects.at(i).getCollisionSide(player->getBoundingBox()) == Global::CollisionSide::BOT)
				{
					moveMapUp();
					player->setPosY(620.0f);
				}
			}
		}


	}

}

void GameMap::moveMapRight()
{
	transitTarget = Vector2(windowAnchor.x, windowAnchor.y + 16);
	transitDir = Vector2(0, 1);
	inTransit = true;
	currentChunk++;
}

void GameMap::moveMapLeft()
{
	transitTarget = Vector2(windowAnchor.x, windowAnchor.y - 16);
	transitDir = Vector2(0, -1);
	inTransit = true;
	currentChunk--;
}

void GameMap::moveMapUp()
{
	transitTarget = Vector2(windowAnchor.x - 11, windowAnchor.y);
	transitDir = Vector2(-1, 0);
	inTransit = true;
	currentChunk -= 3;
}

void GameMap::moveMapDown()
{
	transitTarget = Vector2(windowAnchor.x + 11, windowAnchor.y);
	transitDir = Vector2(1, 0);
	inTransit = true;
	currentChunk += 3;
}

bool GameMap::checkTransit() const
{
	return inTransit;
}

std::vector<Rectangle> GameMap::getChunkCollisions(const Rectangle& other)
{
	std::vector<Rectangle> tmp;
	for (int i = 0; i < mapCollisions[currentChunk].size(); i++)
	{
		if (mapCollisions[currentChunk].at(i).inCollision(other))
		{
			tmp.push_back(mapCollisions[currentChunk].at(i));
		}
	}

	return tmp;
}
