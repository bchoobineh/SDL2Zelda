#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "sprite.h"

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite();
	AnimatedSprite(Graphics& graphics, std::string& filePath, int srcX, int srcY, int width, 
		int height, float posX, float posY);

	~AnimatedSprite();

	void playAnimation(std::string animationName, bool once = false);

	void update(int elapsed);

	void draw(Graphics& graphics, SDL_Rect dst);

protected:
	double timeUntilUpdate;
	bool animationOnce;
	int currentFrame;
	std::string curAnimationName;

	void addAnimation(int numFrames, int x, int y, std::string animName, int width, int height, Vector2 offset, double animSpeed);

	void resetAnimations();

	void stopAnimation();

	void setVisible(bool visible);

	// What happens when an animation is complete
	virtual void animationDone(std::string currentAnimation) = 0;

	// Function to setup all animations
	virtual void setupAnimations() = 0;


private:
	std::map<std::string, std::vector<SDL_Rect>> animationList;
	std::map<std::string, Vector2> animationOffsets;
	std::map<std::string, double> animationSpeeds;

	double elapsedTime;
	bool visible;

};

#endif