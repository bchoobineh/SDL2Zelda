#include "tile.h"

Tile::Tile()
{
	sourceSheet = Tileset();
	textureLocation = Vector2(0, 0);
}

Tile::Tile(Tileset spriteSheet, Vector2 sheetPosition)
{
	sourceSheet = spriteSheet;
	textureLocation = sheetPosition;
}

Tile::~Tile()
{}

void Tile::draw(Graphics& graphics, SDL_Rect* dst)
{
	//printf("Tex Loc: %d %d\n", textureLocation.x, textureLocation.y);

	SDL_Rect src = { textureLocation.x, textureLocation.y, sourceSheet.tileSize.x, sourceSheet.tileSize.y };
	graphics.drawTexture(sourceSheet.tex, &src, dst);
}
