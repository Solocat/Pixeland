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
	//position.x = 0.0;
	//position.y = 0.0;
	origin.x = 0.0;
	origin.y = 0.0;
	hitbox.left = 0;
	hitbox.top = 0;
	hitbox.width = 0;
	hitbox.height = 0;
	airBorne = false;
	freeFall = false;
	airSpeed = 0.0;
	jumpTimeMax = 0.0;
	sprites = nullptr;
}

void Character::setSpritesheet(Spritesheet* sheet)
{
	sprites = sheet;
	sf::Sprite spr;
	spr.setTexture(sheet->fullTex);
	spr.setTextureRect(sheet->getRect(0));
	//spr.setOrigin(spr.getTextureRect().width / 2, spr.getTextureRect().height);
	spr.setOrigin(1, 3);
	sprite = spr;
}

void Character::setHitbox(unsigned w, unsigned h)
{
	hitbox.width = w;
	hitbox.height = h;
	origin.x = (double)(w / 2);
	origin.y = (double)h;
}

bool Character::move(double deltaTime, const Gamemap& map)
{
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


		//double upBound = scanBoundary(Direction::UP, map);

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

	return moved;
}

void Character::moveTo(double x, double y)
{
	position.x = x;
	position.y = y;
	hitbox.left = int(x);
	hitbox.top = int(y);
}

void Character::jumpivate()
{
	airBorne = true;
	startJumpVector = jumpVelocity;
	changeAnim(AnimState::JUMP);
}

double Character::scanDistance(double edge, const std::vector<bool>& map, Direction direction, intVector firstTile, intVector lastTile)
{
	double distance = 0.0;

	//indices of tile to be checked
	int xi = 0;
	int yi = 0;

	//to keep track of smallest value
	int minDist = 1000000;
	int distIndex;

	//for each occupied tile, shoot a ray in desired direction
	//insert smallest value in distance
	for (int i = firstTile.y; i <= lastTile.y; i++)
	{
		for (int j = firstTile.x; j <= lastTile.x; j++)
		{
			yi = i;
			xi = j;
			distIndex = 0;

			while (
				distIndex < minDist
				&& xi >= 0
				&& yi >= 0
				&& xi < 1024
				&& yi < 512
				&& map[yi * 1024 + xi] == false
				)
			{

				switch (direction)
				{
				case Direction::LEFT:	xi--;	break;
				case Direction::RIGHT:	xi++;	break;
				case Direction::UP:	yi--;	break;
				case Direction::DOWN:	yi++;	break;
				}
				distIndex++;
			}
			minDist = min(minDist, distIndex);
		}
	}

	switch (direction)
	{
	case Direction::LEFT:	distance = edge - ++xi;	break;
	case Direction::RIGHT:	distance = xi - edge;	break;
	case Direction::UP:		distance = edge - ++yi;	break;
	case Direction::DOWN:	distance = yi - edge;	break;
	}

	return signbit(distance) ? 0.0 : distance;
}

double Character::scanBoundary(Direction direction, const std::vector<bool>& map)
{
	//scanner's shape is simplified: find every tile which scanner's hitbox overlaps with
	//get the first and last indices of these tiles in both axes
	int x1 =  hitbox.left;
	int x2 = (hitbox.left + hitbox.width - 1);
	int y1 = hitbox.top;
	int y2 = (hitbox.top + hitbox.height - 1);

	intVector tile1;
	intVector tile2;

	double edge; //position of the relevant edge of the hitbox
	switch (direction)
	{
	case Direction::LEFT:
	{
		edge = position.x - origin.x;
		tile1 = { x1,y1 };
		tile2 = { x1,y2 };
		break;
	}
	case Direction::RIGHT:
	{
		edge = position.x - origin.x + hitbox.width;
		tile1 = { x2,y1 };
		tile2 = { x2,y2 };
		break;
	}
	case Direction::UP:
	{
		edge = position.y - origin.y;
		tile1 = { x1,y1 };
		tile2 = { x2,y1 };
		break;
	}
	case Direction::DOWN:
	{
		edge = position.y - origin.y + hitbox.height;
		tile1 = { x1,y2 };
		tile2 = { x2,y2 };
		break;
	}
	default: return 0.0;
	}

	//get maximum distance scanner can travel direction
	return scanDistance(edge, map, direction, tile1, tile2);
}

void Character::render(sf::RenderWindow& window)
{
	sprite.setPosition((int)position.x, (int)position.y);
	window.draw(sprite);
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

void Character::shoot()
{
	
}