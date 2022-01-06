#include "animatedsprite.h"

AnimatedSprite::AnimatedSprite()
{
	currentFrame = 0;
	elapsedTime = 0;
	visible = false;
	timeUntilUpdate = 0;
	animationOnce = false;
	curAnimationName = "";
}

AnimatedSprite::AnimatedSprite(Graphics& graphics, std::string& filePath, int srcX, int srcY, int width,
	int height, float posX, float posY) :
	Sprite(graphics, filePath, srcX, srcY, width, height, posX, posY)
{
	currentFrame = 0;
	timeUntilUpdate = 0;
	visible = true;
	animationOnce = false;
	curAnimationName = "";
	elapsedTime = 0;
}

AnimatedSprite::~AnimatedSprite()
{}

void AnimatedSprite::playAnimation(std::string animationName, bool once)
{
	animationOnce = once;
	if (curAnimationName != animationName)
	{
		curAnimationName = animationName;
		currentFrame = 0;
		elapsedTime = 0;
	}

	timeUntilUpdate = animationSpeeds[curAnimationName];
}

void AnimatedSprite::update(int elapsed)
{
	// Update sprite component
	Sprite::update();

	elapsedTime += elapsed;
	if (elapsedTime > timeUntilUpdate)
	{
		elapsedTime -= timeUntilUpdate;
		if (currentFrame < animationList[curAnimationName].size() - 1)
		{
			currentFrame++;
		}
		else
		{
			if (animationOnce == true)
			{
				setVisible(false);
			}

			stopAnimation();
		}
	}
}

void AnimatedSprite::draw(Graphics& graphics, SDL_Rect dst)
{
	if (visible)
	{
		SDL_Rect src = animationList[curAnimationName][currentFrame];
		graphics.drawTexture(spriteSheet, &src, &dst);
	}
}

void AnimatedSprite::addAnimation(int numFrames, int x, int y, std::string animName, int width, int height, Vector2 offset, double animSpeed)
{
	// Create Animation Rectangles
	std::vector<SDL_Rect> rects;
	for (int i = 0; i < numFrames; i++)
	{
		SDL_Rect tmp = { (i + x) * width, y, width, height };
		rects.push_back(tmp);
	}

	// Update animation and offsets list
	animationList[animName] = rects;
	animationOffsets[animName] = offset;
	animationSpeeds[animName] = animSpeed;
}

void AnimatedSprite::resetAnimations()
{
	animationList.clear();
	animationOffsets.clear();
}

void AnimatedSprite::stopAnimation()
{
	currentFrame = 0;
	animationDone(curAnimationName);
}

void AnimatedSprite::setVisible(bool visible)
{
	this->visible = visible;
}