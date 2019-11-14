#pragma once

#include <vector>

#include "Animation.h"
#include "Gamemap.h"
#include "GameObject.h"
//#include <SFML/Graphics.hpp>
#include <map>

enum class Direction
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum class AnimState
{
	IDLE,
	MOVE,
	JUMP,
	FALL
};



class Character : public KineticObject
{
public:
	Character();

	bool move(double deltaTime, Gamemap& map);
	void jumpivate();
	void animate(double deltaTime);
	void changeAnim(AnimState state);

	doubleVector bulletPosition()
	{
		return 	doubleVector{ 
			position.x + bulletOrigin.x,
			position.y + bulletOrigin.y
		};
	}

	std::map<AnimState, Animation> anims;
	Animation *currentAnim;
	AnimState currentState;
	unsigned currentFrame;
	double frameCounter;
	
	double runSpeed;
	double jumpVelocity;
	double jumpTimeMax;
	
	sf::IntRect hitbox;

	sf::Vector2i bulletOrigin;
};