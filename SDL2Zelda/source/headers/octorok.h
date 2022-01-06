#ifndef OCTOROK_H
#define OCTOROK_H

#include "enemy.h"

class Octorok : public Enemy
{
public:
	Octorok();
	Octorok(Graphics& graphics, std::string& filePath, float posX, float posY);
	~Octorok();

	virtual void draw(Graphics& graphics);
	virtual void update(Uint32 elapsed, std::vector<Rectangle> obst);

	virtual void setupAnimations();
	virtual void animationDone(std::string currentAnimation);

	virtual void processTileCollisions(std::vector<Rectangle>& rects);



private:
	double timer;
	double idleDelay;
	double moveDelay;
	bool idleTrigger;

};

#endif