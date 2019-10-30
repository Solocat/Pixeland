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
	}

	vector<KineticObject> shoot(doubleVector pos, doubleVector velo, double deltaTime, Gamemap& map) {

		if (!triggerHeld) 
		{ 
			timeSinceShot = shootInterval;
			triggerHeld = true;
		}

		timeSinceShot += deltaTime;

		vector<KineticObject> bullets;

		while (timeSinceShot >= shootInterval)
		{
			timeSinceShot -= shootInterval;

			KineticObject bullet = *bulletBase;
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


	const KineticObject* bulletBase;

};