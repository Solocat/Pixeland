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




class PhysicsObject : public GameObject
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

	void inertiamove(double deltaTime)
	{
		position.x += velocity.x * deltaTime;

		fallT += deltaTime;
		position.y = H0height + Y0velocity * fallT + 0.5 * gravity * fallT * fallT;
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

};