#ifndef ENEMY_H
#define ENEMY_H

#include "animatedsprite.h"

class Enemy : public AnimatedSprite
{
public:
	Enemy();
	Enemy(Graphics& graphics, std::string& filePath, float posX, float posY, int health);
	~Enemy();

	virtual void draw(Graphics& graphics);

	virtual void update(Uint32 elapsed, std::vector<Rectangle> obst);

	void death();

	// What happens when an animation is complete
	virtual void animationDone(std::string currentAnimation) = 0;

	// Function to setup all animations
	virtual void setupAnimations() = 0;

	virtual void processTileCollisions(std::vector<Rectangle>& rects) = 0;

	std::vector<Rectangle> getChunkCollisions(const Rectangle& other, std::vector<Rectangle> cols);

	void checkDeath(const Rectangle& other);

protected:
	float dx, dy;
	int currentHealth;
	bool alive;
	Global::Facing currentFace;


};

#endif