#include "rectangle.h"

Rectangle::Rectangle()
{
	x = 0.0f;
	y = 0.0f;
	w = 0.0f;
	h = 0.0f;
}

Rectangle::Rectangle(float xPos, float yPos, float width, float height)
{
	x = xPos;
	y = yPos;
	w = width;
	h = height;
}

Rectangle::~Rectangle()
{

}

float Rectangle::getLeft() const
{
	return x;
}

float Rectangle::getRight() const
{
	return (x + w);
}

float Rectangle::getTop() const
{
	return y;
}

float Rectangle::getBot() const
{
	return (y + h);
}

float Rectangle::getHeight() const
{
	return h;
}

float Rectangle::getWidth() const
{
	return w;
}

bool Rectangle::inCollision(const Rectangle& other) const
{
	return
		getRight() >= other.getLeft() &&
		getLeft() <= other.getRight() &&
		getTop() <= other.getBot() &&
		getBot() >= other.getTop();
}

Global::CollisionSide Rectangle::getCollisionSide(const Rectangle& other) const
{
	int numRight = int(getRight() - other.getLeft());
	int numLeft = int(other.getRight() - getLeft());
	int numTop = int(other.getBot() - getTop());
	int numBot = int(getBot() - other.getTop());

	int tmp[4] = { abs(numRight), abs(numLeft), abs(numTop), abs(numBot) };
	int lowest = tmp[0];
	for (int i = 0; i < 4; i++)
		if (tmp[i] < lowest)
			lowest = tmp[i];


	return
		lowest == abs(numRight) ? Global::CollisionSide::RIGHT :
		lowest == abs(numLeft) ? Global::CollisionSide::LEFT :
		lowest == abs(numTop) ? Global::CollisionSide::TOP :
		lowest == abs(numBot) ? Global::CollisionSide::BOT :
		Global::CollisionSide::NONE;
	

}

void Rectangle::setTop(float yPos)
{
	y = yPos;
}

void Rectangle::setLeft(float xPos)
{
	x = xPos;
}