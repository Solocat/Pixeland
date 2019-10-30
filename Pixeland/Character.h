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

	bool move(double deltaTime, const Gamemap& map);
	void jumpivate();
	void animate(double deltaTime);
	void changeAnim(AnimState state);

	std::map<AnimState, Animation> anims;
	Animation *currentAnim;
	AnimState currentState;
	unsigned currentFrame;
	double frameCounter;
	

	
	double runSpeed;
	double airSpeed;
	double jumpVelocity;
	double jumpTimeMax;
	
	
	

	sf::IntRect hitbox;

	
};