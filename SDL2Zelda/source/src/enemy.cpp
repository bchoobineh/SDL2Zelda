#include "enemy.h"

Enemy::Enemy()
{
	dx = 0.0f;
	dy = 0.0f;
	currentHealth = 0;
	alive = false;
	currentFace = Global::Facing::SOUTH;
}

Enemy::Enemy(Graphics& graphics, std::string& filePath, float posX, float posY, int health):
	AnimatedSprite(graphics, filePath, 0, 0, 16, 16, posX, posY)
{
	dx = 0.0f;
	dy = 0.0f;
	currentHealth = health;
	alive = true;
	currentFace = Global::Facing::SOUTH;
}

Enemy::~Enemy()
{}

void Enemy::draw(Graphics& graphics)
{
	if (alive)
	{
		SDL_Rect dst = { int(xPos), int(yPos) + 192, 64, 64 };
		AnimatedSprite::draw(graphics, dst);
	}
}

void Enemy::update(Uint32 elapsed, std::vector<Rectangle> obst)
{
	xPos += dx * elapsed;
	yPos += dy * elapsed;

	AnimatedSprite::update(elapsed);
}

void Enemy::death()
{
	currentHealth = 0;
	alive = false;
}

std::vector<Rectangle> Enemy::getChunkCollisions(const Rectangle& other, std::vector<Rectangle> cols)
{
	std::vector<Rectangle> tmp;
	for (int i = 0; i < cols.size(); i++)
	{
		if (cols.at(i).inCollision(other))
		{
			tmp.push_back(cols.at(i));
		}
	}

	return tmp;
}

void Enemy::checkDeath(const Rectangle& other)
{
	if (this->getBoundingBox().inCollision(other))
	{
		this->currentHealth--;
		if (currentHealth <= 0)
			this->alive = false;
	}
}