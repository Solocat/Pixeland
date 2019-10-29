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



class Character : public PhysicsObject
{
public:
	Character();

	void setHitbox(unsigned w, unsigned h);
	bool move(double deltaTime, const Gamemap& map);
	void jumpivate();
	double scanDistance(double edge, const std::vector<bool>&, Direction direction, intVector firstTile, intVector lastTile);
	double scanBoundary(Direction direction, const std::vector<bool>&);
	void animate(double deltaTime);
	void changeAnim(AnimState state);
	void shoot();


	std::map<AnimState, Animation> anims;
	Animation *currentAnim;
	AnimState currentState;
	unsigned currentFrame;
	double frameCounter;
	

	
	double runSpeed;
	double airSpeed;
	double jumpVelocity;
	double jumpTimeMax;
	double startJumpVector;
	
	

	sf::IntRect hitbox;

	
};