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
	bool facingRight;

	doubleVector position; //world position. sprite's position must be int
	doubleVector origin; //the local origin of the hitbox
};




class PhysicsObject : public GameObject
{
public:

	void justmove(double deltaTime, const Gamemap& map)
	{
		doubleVector newPos = position;
		position.x += velocity.x * deltaTime;
		position.y += velocity.y * deltaTime;
	}

	doubleVector velocity;
	double gravity;
	double terminalVelocity;

	bool airBorne;
	bool freeFall;
};