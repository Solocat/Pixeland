#pragma once
#include "GameObject.h"
#include "Gamemap.h"

class Gun
{
public:

	Gun()
	{
		shootInterval = 1.0;
		timeSinceShot = 0.0;
		triggerHeld = false;
		bulletBase = nullptr;
	}

	vector<Projectile> shoot(doubleVector pos, doubleVector velo, double deltaTime, Gamemap& map) {

		if (!triggerHeld) 
		{ 
			timeSinceShot = shootInterval;
			triggerHeld = true;
		}

		timeSinceShot += deltaTime;

		vector<Projectile> bullets;

		while (timeSinceShot >= shootInterval)
		{
			timeSinceShot -= shootInterval;

			Projectile bullet = *bulletBase;
			//bullet.gun = this;
			bullet.position = pos;
			bullet.launch(velo.x, velo.y);
			bullet.inertiamove(timeSinceShot, map);

			bullets.push_back(bullet);
		}

		
		return bullets;
	}

	double shootInterval;
	double timeSinceShot;

	bool triggerHeld;


	const Projectile* bulletBase;

};