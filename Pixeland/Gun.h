#pragma once
#include "GameObject.h"

class Gun
{
public:

	Gun()
	{
		shootInterval = 1.0;
		timeSinceShot = 0.0;
		triggerHeld = false;
	}

	vector<PhysicsObject> shoot(doubleVector pos, doubleVector velo, double deltaTime) {

		if (!triggerHeld) 
		{ 
			timeSinceShot = shootInterval;
			triggerHeld = true;
		}

		timeSinceShot += deltaTime;

		vector<PhysicsObject> bullets;

		while (timeSinceShot >= shootInterval)
		{
			timeSinceShot -= shootInterval;

			PhysicsObject bullet = *bulletBase;
			bullet.position = pos;
			bullet.launch(velo.x, velo.y);
			bullet.inertiamove(timeSinceShot);

			bullets.push_back(bullet);
		}

		
		return bullets;
	}

	double shootInterval;
	double timeSinceShot;

	bool triggerHeld;


	const PhysicsObject* bulletBase;

};