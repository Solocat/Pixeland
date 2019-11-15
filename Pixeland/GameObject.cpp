#include "GameObject.h"

void GameObject::render(sf::RenderWindow& window)
{
	if (drawing) 
	{
		drawing->setPosition((float)position.x, (float)position.y);
		window.draw(*drawing);
	}
	else
	{
		sprite.setPosition((float)position.x, (float)position.y);
		window.draw(sprite);
	}
	sf::Drawable *d;
}

void GameObject::setSpritesheet(Spritesheet* sheet)
{
	sprites = sheet;
	sf::Sprite spr;
	spr.setTexture(sheet->fullTex);
	spr.setTextureRect(sheet->getRect(0));
	//spr.setOrigin(spr.getTextureRect().width / 2, spr.getTextureRect().height);
	//spr.setOrigin(1, 3);
	sprite = spr;
}

void GameObject::moveTo(double x, double y)
{
	position.x = x;
	position.y = y;
}

KineticObject::KineticObject()
{
	startHeight = 0;
	startVelocityY = 0;
	airBorne = false;
	freeFall = false;
	airTime = 0.0;
	gravity = 0.0;
	terminalVelocity = 1000.0;
	velocity = doubleVector{ 0,0 };
}

void KineticObject::justmove(double deltaTime)
{
	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
}

void KineticObject::launch(double veloX, double veloY)
{
	velocity.x = veloX;
	velocity.y = veloY;

	startVelocityY = veloY;
	airTime = 0.0;
	startHeight = position.y;
}

bool KineticObject::inertiamove(double deltaTime, Gamemap& map)
{
	if (map.checkCollision(position.x, position.y))
	{
		crashPixel = { (int)position.x, (int)position.y };
		return true;
	}

	doubleVector target;
	target.x = position.x + velocity.x * deltaTime;

	airTime += deltaTime;
	target.y = startHeight + startVelocityY * airTime + 0.5 * gravity * airTime * airTime;

	//move pixel by pixel
	doubleVector pos = pixelMarch((int)target.y, true, map);
	if (pos.x != -1)
	{
		position = pos;
		return true;
	}
	else
	{
		position.y = target.y;
		pos = pixelMarch((int)target.x, false, map);
		if (pos.x != -1)
		{
			position = pos;
			return true;
		}
		else position.x = target.x;
	}
	return false;
}

doubleVector KineticObject::pixelMarch(double goal, bool vertical, Gamemap& map) //return last position before crash, or -1
{
	int increment = 0;
	double dist = goal;

	doubleVector start = position;

	if (vertical) dist -= start.y;
	else dist -= start.x;

	if (dist > 0) increment = 1;
	else if (dist < 0) increment = -1;

	if (dist != 0)
	{
		for (int i = 1; i <= abs(dist) + 1; i++)
		{
			double x = start.x;
			double y = start.y;

			if (vertical) y += increment * i;
			else x += increment * i;

			if (map.isPixelHard((int)x, (int)y))
			{
				crashPixel.x = (int)x;
				crashPixel.y = (int)y;

				if (vertical) y = (int)y - increment;
				else x = (int)x - increment;

				return doubleVector{ x,y };
			}
		}
	}
	return doubleVector{ -1, -1 };
}