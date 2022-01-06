#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "globals.h"

class Rectangle
{
public:
	Rectangle();
	Rectangle(float xPos, float yPos, float width, float height);
	~Rectangle();

	float getLeft() const;
	float getRight() const;
	float getTop() const;
	float getBot() const;

	float getHeight() const;
	float getWidth() const;

	bool inCollision(const Rectangle& other) const;

	Global::CollisionSide getCollisionSide(const Rectangle & other) const;

	void setTop(float yPos);
	void setLeft(float xPos);

private:
	float x, y, w, h;

};

#endif