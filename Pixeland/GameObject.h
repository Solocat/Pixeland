#pragma once
#include <SFML/Graphics.hpp>
#include "Spritesheet.h"
#include "Gamemap.h"

struct intVector
{
	int x;
	int y;
};

struct doubleVector
{
	double x;
	double y;
};

class GameObject
{
public:

	void render(sf::RenderWindow& window);
	void setSpritesheet(Spritesheet* sheet);

	void moveTo(double x, double y);
	sf::Sprite sprite;
	Spritesheet* sprites;
	sf::Drawable* drawing;
	bool facingRight;

	doubleVector position; //world position. sprite's position must be int
	doubleVector origin; //the local origin of the hitbox
};




class KineticObject : public GameObject
{
public:

	void justmove(double deltaTime)
	{
		doubleVector newPos = position;
		position.x += velocity.x * deltaTime;
		position.y += velocity.y * deltaTime;
	}

	void launch(double veloX, double veloY)
	{
		velocity.x = veloX;
		velocity.y = veloY;

		Y0velocity = veloY;
		fallT = 0.0;
		H0height = position.y;
	}

	bool inertiamove(double deltaTime, Gamemap& map)
	{
		if (map.checkCollision(position.x, position.y))
		{
			crashPixel = {(int) position.x, (int)position.y };
			return true;
		}

		doubleVector newpos;
		newpos.x = position.x + velocity.x * deltaTime;

		fallT += deltaTime;
		newpos.y = H0height + Y0velocity * fallT + 0.5 * gravity * fallT * fallT;

		//move pixel by pixel

		doubleVector pos = pixelMarch(position, (int)newpos.y, true, map);
		if (pos.x != -1)
		{
			position = pos;
			return true;
		}
		else
		{
			position.y = newpos.y;
			pos = pixelMarch(position, (int)newpos.x, false, map);
			if (pos.x != -1)
			{
				position = pos;
				return true;
			}
			else position.x = newpos.x;
		}
		return false;

		/*int increment = 0;
		int dist = (int)newpos.y - (int)position.y;
		if (dist > 0) increment = 1;
		else if (dist < 0) increment = -1;

		if (dist != 0)
		{
			for (int y = 1; y <= abs(dist); y++)
			{
				if (map.isPixelHard((int)position.x, (int)position.y + increment*y))
				{
					newpos.y = (int)position.y + increment * (y-1);
					crashPixel.y = (int)position.y + increment * y;
					crashPixel.x = newpos.x = position.x;
					position = newpos;
					return true;
				}
			}
		}
		

		increment = 0;
		dist = (int)newpos.x - (int)position.x;
		if (dist > 0) increment = 1;
		else if (dist < 0) increment = -1;


		if (dist != 0)
		{
			for (int x = 1; x <= abs(dist); x++)
			{
				if (map.isPixelHard((int)position.x + increment*x, (int)newpos.y))
				{
					newpos.x = (int)position.x + increment * (x-1);
					crashPixel.x = (int)position.x + increment * x;
					crashPixel.y = newpos.y;
					position = newpos;
					return true;
				}
			}
		}*/

		//position = newpos;
		//return false;
	}

	doubleVector pixelMarch(doubleVector start, int goal, bool vertical, Gamemap& map) //return last position before crash, or -1
	{
		int increment = 0;
		int dist = goal;

		if (vertical) dist -= (int)start.y;
		else dist -= (int)start.x;

		if (dist > 0) increment = 1;
		else if (dist < 0) increment = -1;

		if (dist != 0)
		{
			for (int i = 1; i <= abs(dist); i++)
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

					return doubleVector{x,y};
				}
			}
		}
		return doubleVector{ -1, -1 };
	}

	double Y0velocity;
	double fallT;
	double H0height;

	double startJumpVector;
	doubleVector velocity;
	double gravity;
	double terminalVelocity;

	bool airBorne;
	bool freeFall;

	double explosionRadius;
	bool creative = false;
	sf::Vector2i crashPixel;
};