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
		bool collision = false;
		

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
		if (newpos.y > position.y)
		{
			for (int y = (int)position.y; y <= (int)newpos.y; y++) 
			{
				if (map.isPixelHard((int)position.x, y))
				{
					newpos.y = y-1;	
					crashPixel.y = y;
					crashPixel.x = newpos.x = position.x;
					collision = true;
					break;
				}
			}
		}
		else
		{
			for (int y = (int)position.y; y >= (int)newpos.y; y--)
			{
				if (map.isPixelHard((int)position.x, y))
				{
					newpos.y = y+1;
					crashPixel.y = y;
					crashPixel.x = newpos.x = position.x;
					collision = true;	
					break;
				}
			}
		}

		if (!collision)
		{
			crashPixel.y = newpos.y;
			if (newpos.x > position.x)
			{
				for (int x = (int)position.x; x <= (int)newpos.x; x++)
				{
					if (map.isPixelHard(x, (int)newpos.y))
					{
						newpos.x = x - 1;
						crashPixel.x = x;
						collision = true;
						break;
					}
				}
			}
			else
			{
				for (int x = (int)position.x; x >= (int)newpos.x; x--)
				{
					if (map.isPixelHard(x, (int)newpos.y))
					{
						newpos.x = x + 1;
						crashPixel.x = x;
						collision = true;
						break;
					}
				}
			}
		}

		position = newpos;

		return collision;
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