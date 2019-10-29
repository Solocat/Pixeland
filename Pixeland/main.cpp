#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Gun.h"
#include <vector>
#include <list>

int main()
{
	Window window("Platform", 1024, 512, 60);
	window.camera.zoom(0.25);

	Gamemap map("dirtmap.png");

	Character player;
	player.gravity = 1500.0;
	player.runSpeed = 10 * 8;
	player.jumpVelocity = 22 * 8;
	player.jumpTimeMax = 0.14;
	player.terminalVelocity = 22 * 8;

	Spritesheet playerSprites("soldier.png", 3, 4, true);
	player.setSpritesheet(&playerSprites);
	player.sprite.setOrigin(1, 3);

	player.anims[AnimState::IDLE] = Animation(0, 0, 0.0);
	player.anims[AnimState::MOVE] = Animation(0, 0, 0.01875);
	player.anims[AnimState::JUMP] = Animation(0, 0, 0.0);
	player.anims[AnimState::FALL] = Animation(0, 0, 0.0);
	player.changeAnim(AnimState::IDLE);
	
	player.moveTo(200, 200);

	Spritesheet bulletSprites("redpixel.png", 1, 1, false);
	list<PhysicsObject> projectiles;

	PhysicsObject bullet;
	bullet.gravity = 500.0;
	bullet.explosionRadius = 5.0;
	bullet.setSpritesheet(&bulletSprites);
	Gun magnum;
	magnum.shootInterval = 0.5;
	magnum.bulletBase = &bullet;

	PhysicsObject slime;
	slime.gravity = 500.0;
	slime.explosionRadius = 1.0;
	slime.setSpritesheet(&bulletSprites);
	Gun slimer;
	slimer.shootInterval = 0.01;
	slimer.bulletBase = &slime;

	sf::Clock clock;
	bool quit = false;


	while (!quit)
	{
		sf::Time frameTime = clock.restart();
		if (frameTime.asMilliseconds() > 100) frameTime = sf::milliseconds(100);	//at low framerates game becomes fps dependent to avoid collision errors etc.

		//event block
		sf::Event e;
		while (window.win.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
			{
				quit = true;
				break;
			}
			case sf::Event::KeyPressed:
			{
				switch (e.key.code)
				{
				case sf::Keyboard::A:
				case sf::Keyboard::Left:
				{
					player.velocity.x = -player.runSpeed;
					break;
				}
				case sf::Keyboard::D:
				case sf::Keyboard::Right:
				{
					player.velocity.x = player.runSpeed;
					break;
				}
				default: break;
				}
				break;
			}
			case sf::Event::KeyReleased:
			{
				switch (e.key.code)
				{
				case sf::Keyboard::A:
				case sf::Keyboard::Left:
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player.velocity.x = player.runSpeed;
					else player.velocity.x = 0.0;
					break;
				}
				case sf::Keyboard::D:
				case sf::Keyboard::Right:
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player.velocity.x = -player.runSpeed;
					else player.velocity.x = 0.0;
					break;
				}
				default: break;
				}
				break;
			}
			case sf::Event::MouseButtonPressed: 
			{	
				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				magnum.triggerHeld = false;
				slimer.triggerHeld = false;
				break;
			}
			default: break;
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i pointA = window.win.mapCoordsToPixel(player.sprite.getPosition());
			sf::Vector2i pointB = sf::Mouse::getPosition(window.win);

			vector<PhysicsObject> bullets = magnum.shoot(player.position, { double(pointB.x - pointA.x), double(pointB.y - pointA.y) }, frameTime.asSeconds());

			for (auto& i : bullets)
			{
				projectiles.push_back(i);
			}		
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			sf::Vector2i pointA = window.win.mapCoordsToPixel(player.sprite.getPosition());
			sf::Vector2i pointB = sf::Mouse::getPosition(window.win);

			vector<PhysicsObject> bullets = slimer.shoot(player.position, { double(pointB.x - pointA.x), double(pointB.y - pointA.y) }, frameTime.asSeconds());

			for (auto& i : bullets)
			{
				projectiles.push_back(i);
			}
		}

		//movement block
		if (!player.freeFall)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player.jumpivate();
			}
			else if (player.airBorne)
			{
				player.freeFall = true;
			}
		}

		if (player.move(frameTime.asSeconds(), map))
		{
			window.follow((int)player.position.x, (int)player.position.y, sf::Vector2i(map.size), 4*8);
		}
		player.animate(frameTime.asSeconds());

		vector<list<PhysicsObject>::iterator> deletions;
		for (auto i = projectiles.begin(); i != projectiles.end(); i++)
		{
			i->inertiamove(frameTime.asSeconds());
			i->velocity.y += frameTime.asSeconds() * i->gravity;
			if (map.checkCollision(i->position.x, i->position.y))
			{
				if (i->explosionRadius <= 1.0)
				{
					map.pixelExplosion(i->position.x, i->position.y);
				}
				else map.circleExplosion(i->position.x, i->position.y, i->explosionRadius);
				
				deletions.push_back(i);
			}
		}
		for (auto& i : deletions)
		{
			projectiles.erase(i);
		}
		deletions.clear();

		//render block
		window.win.clear();
		window.win.draw(map.spr);
		player.render(window.win);

		for (auto& i : projectiles)
		{
			i.render(window.win);
		}

		window.win.display();
	}
}