#pragma once
#include "GameObject.h"
#include "Gamemap.h"

class Gun
{
public:

	Gun()
	{
		shootInterval = 1.0;
		bulletBase = nullptr;
	}

	Projectile shoot(doubleVector pos, doubleVector velo, Gamemap& map)
	{
		if (timer.getElapsedTime().asSeconds() >= shootInterval)
		{
			timer.restart();
			Projectile bullet = *bulletBase;
			bullet.position = pos;
			bullet.launch(velo.x, velo.y);
			
			return bullet;
		}
	}

	double shootInterval;
	sf::Clock timer;

	const Projectile* bulletBase;

};