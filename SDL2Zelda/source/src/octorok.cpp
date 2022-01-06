#include "octorok.h"

Octorok::Octorok() :
	Enemy()
{
	idleTrigger = false;
	timer = 0;
	idleDelay = 0;
	moveDelay = 0;
}

Octorok::Octorok(Graphics& graphics, std::string& filePath, float posX, float posY) :
	Enemy(graphics, filePath, posX, posY, 1)
{
	graphics.loadSpriteSheet(filePath);
	setupAnimations();
	playAnimation("WalkNorth");
	currentFace = Global::Facing::NORTH;

	// Timers
	timer = 0;
	idleDelay = 1000;
	moveDelay = 3000;
	idleTrigger = false;
}

Octorok::~Octorok()
{

}

void Octorok::draw(Graphics& graphics)
{
	if(this->alive)
		Enemy::draw(graphics);
}


void Octorok::setupAnimations()
{
	addAnimation(2, 0, 0, "WalkSouth", 16, 16, Vector2(0, 0), 300);
	addAnimation(2, 0, 16, "WalkNorth", 16, 16, Vector2(0, 0), 300);
	addAnimation(2, 0, 32, "WalkWest", 16, 16, Vector2(0, 0), 300);
	addAnimation(2, 0, 48, "WalkEast", 16, 16, Vector2(0, 0), 300);
}

void Octorok::animationDone(std::string currentAnimation)
{

}

void Octorok::update(Uint32 elapsed, std::vector<Rectangle> obst)
{
	timer += elapsed;

	// In an Idle Standing Phase
	if (!idleTrigger)
	{
		// Leave Idle
		if (timer >= idleDelay)
		{
			timer -= idleDelay;
			idleTrigger = true;

			if (currentFace == Global::Facing::NORTH)
				this->playAnimation("WalkNorth");
			else if (currentFace == Global::Facing::SOUTH)
				this->playAnimation("WalkSouth");
		}

		this->dx = 0;
		this->dy = 0;
	}

	// Moving
	else
	{
		if (timer >= moveDelay)
		{
			timer -= moveDelay;
			idleTrigger = false;
			this->dx = 0;
			this->dy = 0;

			if (this->currentFace == Global::Facing::SOUTH)
			{
				this->currentFace = Global::Facing::NORTH;
			}
			else if (this->currentFace == Global::Facing::NORTH)
			{
				this->currentFace = Global::Facing::SOUTH;
			}
		}

		else
		{
			if (this->currentFace == Global::Facing::SOUTH)
				this->dy = 0.1f;
			else if (this->currentFace == Global::Facing::NORTH)
				this->dy = -0.1f;
		}
	}


	Enemy::update(elapsed, obst);

	// Handle Level Collisions
	std::vector<Rectangle> tmp;
	if ((tmp = getChunkCollisions(this->getBoundingBox(), obst)).size() > 0)
	{
		processTileCollisions(tmp);
	}
	


}

void Octorok::processTileCollisions(std::vector<Rectangle>& rects)
{
	for (int i = 0; i < rects.size(); i++)
	{
		Global::CollisionSide side = this->getBoundingBox().getCollisionSide(rects.at(i));
		if (side != Global::CollisionSide::NONE)
		{
			switch (side)
			{
			case Global::CollisionSide::TOP:
				dy = 0;
				yPos = rects.at(i).getBot() + 1;
				break;

			case Global::CollisionSide::BOT:
				dy = 0;
				yPos = rects.at(i).getTop() - getBoundingBox().getHeight() - 1;
				break;

			case Global::CollisionSide::LEFT:
				dx = 0;
				xPos = rects.at(i).getRight() + 1;
				break;

			case Global::CollisionSide::RIGHT:
				dx = 0;
				xPos = rects.at(i).getLeft() - getBoundingBox().getWidth() - 1;
				break;

			}
		}
	}
}
