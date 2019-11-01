#include "Character.h"

int min(int a, int b)
{
	return a < b ? a : b;
}


Character::Character()
{
	currentFrame = 0;
	frameCounter = 0.0;
	facingRight = true;
	currentState = AnimState::IDLE;
	currentAnim = nullptr;
	velocity.x = 0.0;
	velocity.y = 0.0;
	gravity = 0.0;
	runSpeed = 0.0;
	jumpVelocity = 0.0;
	startJumpVector = 0.0;
	terminalVelocity = 0.0;
	position.x = 0.0;
	position.y = 0.0;
	origin.x = 0.0;
	origin.y = 0.0;
	hitbox.left = 0;
	hitbox.top = 0;
	hitbox.width = 0;
	hitbox.height = 0;
	airBorne = false;
	freeFall = false;
	jumpTimeMax = 0.0;
	sprites = nullptr;
}

bool Character::move(double deltaTime, const Gamemap& map)
{
	if (map.checkCollision(position.x, position.y)) //player is stuck
	{
		freeFall = false;
		airBorne = false;
		return false;
	}

	bool moved = false;	//character has moved a whole pixel *clapclap*
	////////////////Y_AXIS///////////////////////////
	static double startHeight = position.y;
	//double downBound = scanBoundary(Direction::DOWN, map);

	int oldY = (int)position.y;

	if (!airBorne)	//grounded
	{
		if (!map.isPixelHard((int)position.x, (int)position.y + 1))	//fall through
		{
			airBorne = true;
			freeFall = true;
			changeAnim(AnimState::FALL);
		}
	}

	if (airBorne)	//airborne
	{
		double targetPos;
		static double airTime = 0.0;
		static double fallTime = 0.0;

		airTime += deltaTime;
		if (!freeFall)	//actively jump
		{
			if (airTime >= jumpTimeMax)	//max jump
			{
				freeFall = true;
				deltaTime = airTime - jumpTimeMax;	//freefall for the remaining time
			}
			else targetPos = startHeight - jumpVelocity*airTime;
		}

		if (freeFall)	//freefall
		{
			fallTime += deltaTime;
			targetPos = startHeight - startJumpVector*airTime + 0.5*gravity*fallTime*fallTime;

			if (targetPos - position.y > terminalVelocity*deltaTime)	//terminal velocity
			{
				targetPos = position.y + terminalVelocity*deltaTime;
			}
		}

		double dist = targetPos - position.y;

		if (targetPos < position.y)
		{	
			if (map.isPixelHard(int(position.x), (int)targetPos))	//hit ceiling
			{
				position.y = (int)targetPos + 1;
				freeFall = true;
				startHeight = position.y;
				startJumpVector = 0.0;
				airTime = 0.0;
				fallTime = 0.0;
				changeAnim(AnimState::FALL);
			}
			else
			{
				position.y = targetPos;
				changeAnim(AnimState::JUMP);
			}
		}
		else
		{
			if (map.isPixelHard(int(position.x), (int)targetPos))	//landing
			{
				position.y = (int)targetPos - 1;
				airBorne = false;
				freeFall = false;
				startHeight = position.y;
				startJumpVector = 0.0;
				airTime = 0.0;
				fallTime = 0.0;
				changeAnim(AnimState::IDLE);
			}
			else
			{
				position.y = targetPos;
				changeAnim(AnimState::FALL);
			}
		}
		

		if (oldY != (int)position.y)
		{
			moved = true;
		}
	}


	///////////////////////X-Axis//////////////////////////
	int targetDist = 0;

	if (velocity.x != 0.0)
	{
		int oldX = (int)position.x;
		position.x += velocity.x * deltaTime;
		int newX = (int)position.x;

		
		if (map.isPixelHard(newX, (int)position.y)) //hit hard pixel
		{
			if (!map.isPixelHard(newX, (int)position.y - 1)) //climb up
			{
				position.y = (int)position.y - 1;
				moved = true;
			}
			else
			{
				position.x = newX = oldX;
			}
		}
		else moved = true;

		if (currentState == AnimState::IDLE)
		{
			changeAnim(AnimState::MOVE);
		}
	}
	else if (!airBorne)
	{
		changeAnim(AnimState::IDLE);
	}

	moved = true;
	return moved;
}

void Character::jumpivate()
{
	airBorne = true;
	startJumpVector = jumpVelocity;
	changeAnim(AnimState::JUMP);
}

void Character::animate(double deltaTime)
{
	if (currentAnim == nullptr) return;

	unsigned oldFrame = currentFrame;
	currentFrame = currentAnim->animate(deltaTime, currentFrame, frameCounter);

	if (currentFrame == oldFrame) return;

	sprite.setTextureRect(sprites->getRect(currentFrame));
}

void Character::changeAnim(AnimState state)
{
	if (state == currentState) return;
	if (!anims.count(state)) return; //animation not found
	currentState = state;
	currentAnim = &anims[state];

	currentFrame = currentAnim->startFrame;
	sprite.setTextureRect(sprites->getRect(currentFrame));

	frameCounter = 0.0;
}