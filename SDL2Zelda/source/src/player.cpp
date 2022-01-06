#include "player.h"

Player::Player()
{
	dx = 0.0f;
	dy = 0.0f;
	isAttacking = false;
	currentFace = Global::Facing::SOUTH;
}

Player::Player(Graphics& graphics, std::string& filePath) :
	AnimatedSprite(graphics, filePath, 0, 0, 16, 16, 70, 300)
{
	dx = 0;
	dy = 0;

	isAttacking = false;

	// Load Sprite Sheet
	graphics.loadSpriteSheet(filePath);

	setupAnimations();
	playAnimation("IdleSouth");

	currentFace = Global::Facing::SOUTH;

	// Import Sword
	equippedSword.southSource = { 8, 128, 8, 16 };
	equippedSword.northSource = { 0, 128, 8, 16 };
	equippedSword.eastSource = { 16, 128, 16, 16 };
	equippedSword.westSource = { 32, 128, 16, 16 };


	equippedSword.boundingBox = this->getBoundingBox();
	equippedSword.width = 8;
	equippedSword.height = 16;
	equippedSword.xPos = int(this->xPos);
	equippedSword.yPos = int(this->yPos);


}

Player::~Player()
{}

void Player::draw(Graphics& graphics)
{
	if(isAttacking)
		drawSword(graphics);

	SDL_Rect dst = { int(xPos), int(yPos) + 192, 64, 64 };
	AnimatedSprite::draw(graphics, dst);
}

void Player::drawSword(Graphics& graphics)
{
	SDL_Rect dst;
	if(this->curAnimationName == "AttackSouth" || this->curAnimationName == "AttackNorth")
		dst = { equippedSword.xPos + 16, equippedSword.yPos + 192, equippedSword.width * 4, equippedSword.height * 4 };
	else
		dst = { equippedSword.xPos, equippedSword.yPos + 192, equippedSword.width * 8, equippedSword.height * 4 };


	if (this->curAnimationName == "AttackSouth")
		graphics.drawTexture(this->spriteSheet, &equippedSword.southSource, &dst);
	else if (this->curAnimationName == "AttackNorth")
		graphics.drawTexture(this->spriteSheet, &equippedSword.northSource, &dst);
	else if (this->curAnimationName == "AttackEast")
		graphics.drawTexture(this->spriteSheet, &equippedSword.eastSource, &dst);
	else
		graphics.drawTexture(this->spriteSheet, &equippedSword.westSource, &dst);

}

void Player::handleInput(std::vector<SDL_Event> inp)
{
	for (int i = 0; i < inp.size(); i++)
	{
		if (inp.at(i).type == SDL_KEYDOWN)
		{
			if (inp.at(i).key.keysym.scancode == SDL_SCANCODE_D)
			{
				dx = 0.2f;

			}
			else if (inp.at(i).key.keysym.scancode == SDL_SCANCODE_A)
			{
				dx = -0.2f;

			}

			if (inp.at(i).key.keysym.scancode == SDL_SCANCODE_W)
			{
				dy = -0.2f;

			}
			else if (inp.at(i).key.keysym.scancode == SDL_SCANCODE_S)
			{
				dy = 0.2f;
			}

			else if (inp.at(i).key.keysym.scancode == SDL_SCANCODE_SPACE)
			{
				if (inp.at(i).key.repeat == 0)
				{
					// Attack
					if (currentFace == Global::Facing::NORTH)
						playAnimation("AttackNorth");
					else if (currentFace == Global::Facing::EAST)
						playAnimation("AttackEast");
					else if (currentFace == Global::Facing::WEST)
						playAnimation("AttackWest");
					else
						playAnimation("AttackSouth", true);


					isAttacking = true;
				}
			}

		}

		else if (inp.at(i).type == SDL_KEYUP)
		{
			if (inp.at(i).key.keysym.scancode == SDL_SCANCODE_D || inp.at(i).key.keysym.scancode == SDL_SCANCODE_A)
			{
				dx = 0.f;
			}

			if (inp.at(i).key.keysym.scancode == SDL_SCANCODE_W || inp.at(i).key.keysym.scancode == SDL_SCANCODE_S)
			{
				dy = 0.f;
			}

		}

		// Handle Animations
		// Horizontal Movement
		if (dx != 0 && dy == 0)
		{
			if (dx > 0)
			{
				playAnimation("WalkEast");
				currentFace = Global::Facing::EAST;

			}
			else
			{
				playAnimation("WalkWest");
				currentFace = Global::Facing::WEST;
			}
		}

		// Vertial Movement
		else if (dy != 0 && dx == 0)
		{
			if (dy > 0)
			{
				playAnimation("WalkSouth");
				currentFace = Global::Facing::SOUTH;
			}
			else
			{
				playAnimation("WalkNorth");
				currentFace = Global::Facing::NORTH;
			}
		}
	}
}

void Player::update(Uint32 elapsed)
{
	// Handle Movement Stop Animation
	if (dx == 0 && dy == 0 && isAttacking == false)
	{
		if (curAnimationName == "WalkSouth")
			playAnimation("IdleSouth");
		else if (curAnimationName == "WalkEast")
			playAnimation("IdleEast");
		else if (curAnimationName == "WalkNorth")
			playAnimation("IdleNorth");
		else if (curAnimationName == "WalkWest")
			playAnimation("IdleWest");
	}

	// Update Player position
	xPos += dx * elapsed;
	yPos += dy * elapsed;





	// Update Sword Location	
	if (this->curAnimationName == "AttackSouth")
	{

		if (this->currentFrame == 0)
		{
			equippedSword.xPos = int(xPos);
			equippedSword.yPos = int(yPos);
		}
		else if (this->currentFrame == 1)
		{
			equippedSword.yPos = int(this->yPos + 44);
			equippedSword.boundingBox.setTop(this->getBoundingBox().getTop() + 44);
		}
		else if (this->currentFrame == 2)
		{
			equippedSword.yPos = int(this->yPos + 28);
			equippedSword.boundingBox.setTop(this->getBoundingBox().getTop() + 28);
		}
		else if (this->currentFrame == 3)
		{
			equippedSword.yPos = int(this->yPos + 12);
			equippedSword.boundingBox.setTop(this->getBoundingBox().getTop() + 12);
		}
	}

	else if (this->curAnimationName == "AttackNorth")
	{

		if (this->currentFrame == 0)
		{
			equippedSword.xPos = int(xPos);
			equippedSword.yPos = int(yPos);
		}
		else if (this->currentFrame == 1)
		{
			equippedSword.yPos = int(this->yPos - 44);
			equippedSword.boundingBox.setTop(this->getBoundingBox().getTop() - 44);
		}
		else if (this->currentFrame == 2)
		{
			equippedSword.yPos = int(this->yPos - 28);
			equippedSword.boundingBox.setTop(this->getBoundingBox().getTop() - 28);
		}
		else if (this->currentFrame == 3)
		{
			equippedSword.yPos = int(this->yPos - 12);
			equippedSword.boundingBox.setTop(this->getBoundingBox().getTop() - 12);
		}
	}

	else if (this->curAnimationName == "AttackEast")
	{

		if (this->currentFrame == 0)
		{
			equippedSword.xPos = int(xPos);
			equippedSword.yPos = int(yPos);
		}
		else if (this->currentFrame == 1)
		{
			equippedSword.xPos = int(this->xPos + 44);
			equippedSword.boundingBox.setLeft(this->getBoundingBox().getLeft() + 44);
		}
		else if (this->currentFrame == 2)
		{
			equippedSword.xPos = int(this->xPos + 28);
			equippedSword.boundingBox.setLeft(this->getBoundingBox().getLeft() + 28);
		}
		else if (this->currentFrame == 3)
		{
			equippedSword.xPos = int(this->xPos + 12);
			equippedSword.boundingBox.setLeft(this->getBoundingBox().getLeft() + 12);
		}
	}

	else if (this->curAnimationName == "AttackWest")
	{

		if (this->currentFrame == 0)
		{
			equippedSword.xPos = int(xPos);
			equippedSword.yPos = int(yPos);
		}
		else if (this->currentFrame == 1)
		{
			equippedSword.xPos = int(this->xPos - 44);
			equippedSword.boundingBox.setLeft(this->getBoundingBox().getLeft() + 44);
		}
		else if (this->currentFrame == 2)
		{
			equippedSword.xPos = int(this->xPos - 28);
			equippedSword.boundingBox.setLeft(this->getBoundingBox().getLeft() + 28);
		}
		else if (this->currentFrame == 3)
		{
			equippedSword.xPos = int(this->xPos - 12);
			equippedSword.boundingBox.setLeft(this->getBoundingBox().getLeft() + 12);
		}
	}



	else
	{
		equippedSword.boundingBox.setTop(this->getBoundingBox().getTop());
		isAttacking = false;
		equippedSword.xPos = int(xPos);
		equippedSword.yPos = int(yPos);
	}

	AnimatedSprite::update(elapsed);

}

float Player::getPosX() const
{
	return xPos;
}

float Player::getPosY() const
{
	return yPos;
}

void Player::setPosX(float x)
{
	xPos = x;
	dx = 0;
	dy = 0;
}

void Player::setPosY(float y)
{
	yPos = y;
	dx = 0;
	dy = 0;
}

void Player::setupAnimations()
{
	addAnimation(1, 0, 0, "IdleSouth", 16, 16, Vector2(0, 0), 300);
	addAnimation(1, 0, 16, "IdleEast", 16, 16, Vector2(0, 0), 300);
	addAnimation(1, 0, 32, "IdleNorth", 16, 16, Vector2(0, 0), 300);
	addAnimation(1, 0, 48, "IdleWest", 16, 16, Vector2(0, 0), 300);

	addAnimation(2, 0, 0, "WalkSouth", 16, 16, Vector2(0, 0), 300);
	addAnimation(2, 0, 16, "WalkEast", 16, 16, Vector2(0, 0), 300);
	addAnimation(2, 0, 32, "WalkNorth", 16, 16, Vector2(0, 0), 300);
	addAnimation(2, 0, 48, "WalkWest", 16, 16, Vector2(0, 0), 300);

	addAnimation(4, 0, 64, "AttackSouth", 16, 16, Vector2(0, 0), 80);
	addAnimation(4, 0, 80, "AttackEast", 16, 16, Vector2(0, 0), 100);
	addAnimation(4, 0, 96, "AttackNorth", 16, 16, Vector2(0, 0), 100);
	addAnimation(4, 0, 112, "AttackWest", 16, 16, Vector2(0, 0), 100);


}

void Player::animationDone(std::string currentAnimation)
{
	setVisible(true);

	if (currentFace == Global::Facing::SOUTH)
		playAnimation("IdleSouth");
	else if (currentFace == Global::Facing::EAST)
		playAnimation("IdleEast");
	else if (currentFace == Global::Facing::WEST)
		playAnimation("IdleWest");
	else
		playAnimation("IdleNorth");


}

void Player::processTileCollisions(std::vector<Rectangle>& rects)
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

Rectangle Player::getSwordBox()
{
	return (equippedSword.boundingBox);
}