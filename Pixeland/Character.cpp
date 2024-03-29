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
	fallTime = 0.0;
}

bool Character::move(double deltaTime, Gamemap& map)
{
	if (map.checkCollision(position.x, position.y)) //player is stuck. This shouldn't happen.
	{
		cout << "Player is stuck!";
		freeFall = false;
		airBorne = false;
		return false;
	}

	////////////////Y_AXIS///////////////////////////

	if (!airBorne)	//grounded
	{
		if (!map.isPixelHard((int)position.x, (int)position.y + 1))	//fall through
		{
			airBorne = true;
			freeFall = true;
			startHeight = position.y;
			changeAnim(AnimState::FALL);
		}
	}

	if (airBorne)	//airborne
	{
		double target;

		airTime += deltaTime;
		if (!freeFall)	//actively jump
		{
			if (airTime >= jumpTimeMax)	//max jump
			{
				freeFall = true;
				deltaTime = airTime - jumpTimeMax;	//freefall for the remaining time
			}
			else target = startHeight - jumpVelocity*airTime;
		}

		if (freeFall)	//freefall
		{
			fallTime += deltaTime;
			target = startHeight - startVelocityY *airTime + 0.5*gravity*fallTime*fallTime;

			if (target - position.y > terminalVelocity*deltaTime)	//terminal velocity
			{
				target = position.y + terminalVelocity*deltaTime;
			}
		}

		if (pixelMarch(target, true, map)) //collision
		{
			startHeight = position.y;
			startVelocityY = 0.0;
			airTime = 0.0;
			fallTime = 0.0;

			if (target < position.y) //hit ceiling
			{
				freeFall = true;
				changeAnim(AnimState::FALL);
			}
			else //landing
			{
				airBorne = false;
				freeFall = false;
				changeAnim(AnimState::IDLE);
			}
		}
	}

	///////////////////////X-Axis//////////////////////////

	if (velocity.x != 0.0)
	{
		double target = position.x + velocity.x * deltaTime;
		
		climbMarch(target, map);

		if (currentState == AnimState::IDLE)
		{
			changeAnim(AnimState::MOVE);
		}
	}
	else if (!airBorne)
	{
		changeAnim(AnimState::IDLE);
	}
}

bool Character::climbMarch(double goal, Gamemap& map) //return true if crash
{
	int increment = 0;
	double dist = goal - position.x;

	if (dist > 0) increment = 1;
	else if (dist < 0) increment = -1;
	else return false;

	int pos = (int)position.x;
	for (int i = 1; i <= abs(dist) + 1; i++)
	{
		pos += increment * i;

		if (map.isPixelHard(pos, (int)position.y))
		{
			if (map.isPixelHard(pos, (int)position.y - 1))
			{
				crashPixel.x = pos;
				crashPixel.y = (int)position.y;

				position.x = pos - increment;
				return true;
			}
			else
			{
				position.y--;
			}
		}
	}
	position.x = goal;
	return false;
}

void Character::jumpivate()
{
	airBorne = true;
	startVelocityY = jumpVelocity;
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