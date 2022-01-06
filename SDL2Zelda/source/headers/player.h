#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"

struct Sword
{
	SDL_Rect northSource;
	SDL_Rect eastSource;
	SDL_Rect westSource;
	SDL_Rect southSource;

	int width, height;
	int xPos, yPos;

	Rectangle boundingBox;

	Sword() :
		northSource({ 0,0,0,0 }),
		eastSource({ 0,0,0,0 }),
		westSource({ 0,0,0,0 }),
		southSource({ 0,0,0,0 }),
		width(0),
		height(0),
		xPos(0),
		yPos(0)
	{}


};

class Player : public AnimatedSprite
{
public:
	Player();
	Player(Graphics& graphics, std::string& filePath);
	~Player();

	void draw(Graphics& graphics);
	void drawSword(Graphics& graphics);
	
	void handleInput(std::vector<SDL_Event> inp);

	void update(Uint32 elapsed);

	float getPosX() const;
	float getPosY() const;

	void setPosX(float x);
	void setPosY(float y);

	virtual void setupAnimations();
	virtual void animationDone(std::string currentAnimation);

	Rectangle getSwordBox();

	void processTileCollisions(std::vector<Rectangle>& rects);

private:
	float dx, dy;
	bool isAttacking;
	Global::Facing currentFace;
	Sword equippedSword;


};

#endif