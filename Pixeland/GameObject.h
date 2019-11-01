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

	GameObject()
	{
		sprites = nullptr;
		position = doubleVector{ 0,0 };
		origin = doubleVector{ 0,0 };
		facingRight = true;
		drawing = nullptr;
	}

	void render(sf::RenderWindow& window);
	void setSpritesheet(Spritesheet* sheet);

	void moveTo(double x, double y);
	sf::Sprite sprite;
	Spritesheet* sprites;
	sf::Shape *drawing;
	bool facingRight;

	doubleVector position; //world position
	doubleVector origin; //the local origin of the hitbox
};


class KineticObject : public GameObject
{
public:

	KineticObject();

	void justmove(double deltaTime);
	void launch(double veloX, double veloY);
	bool inertiamove(double deltaTime, Gamemap& map);
	doubleVector pixelMarch(doubleVector start, int goal, bool vertical, Gamemap& map); //return last position before crash, or -1

	double Y0velocity;
	double fallT;
	double H0height;

	double startJumpVector;
	doubleVector velocity;
	double gravity;
	double terminalVelocity;

	bool airBorne;
	bool freeFall;

	
	sf::Vector2i crashPixel;
};

class Projectile : public KineticObject
{
public:

	double explosionRadius = 1.0;
	bool creative = false;

	//const Gun* gun;
};