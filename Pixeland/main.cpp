#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Gun.h"
#include <vector>
#include <list>

int main()
{
	Window window("Platform", 1024, 512, 60);
	window.zoom(4);

	Gamemap map("dirtmap.png");

	Character player;
	player.gravity = 500.0;
	player.runSpeed = 10 * 8;
	player.jumpVelocity = 16 * 8;
	player.jumpTimeMax = 0.14;
	player.terminalVelocity = 16 * 8;

	Spritesheet playerSprites("soldier.png", 3, 4, true);
	player.setSpritesheet(&playerSprites);
	player.sprite.setOrigin(1, 3);
	player.bulletOrigin = sf::Vector2i(0, -2);

	player.anims[AnimState::IDLE] = Animation(0, 0, 0.0);
	player.anims[AnimState::MOVE] = Animation(0, 0, 0.01875);
	player.anims[AnimState::JUMP] = Animation(0, 0, 0.0);
	player.anims[AnimState::FALL] = Animation(0, 0, 0.0);
	player.changeAnim(AnimState::IDLE);
	
	player.moveTo(200, 200);
	player.startHeight = player.position.y;

	Spritesheet bulletSprites("redpixel.png", 1, 1, false);
	Spritesheet greenPix("greenpixel.png", 1, 1, false);
	list<Projectile> projectiles;

	sf::RectangleShape redpix;
	redpix.setSize({ 1, 1 });
	redpix.setFillColor(sf::Color::Red);

	Projectile bullet;
	bullet.gravity = 500.0;
	bullet.explosionRadius = 5.0;
	//bullet.setSpritesheet(&bulletSprites);
	bullet.drawing = &redpix;
	bullet.onHit = &Gamemap::circleExplosion;
	Gun magnum;
	magnum.shootInterval = 0.5;
	magnum.bulletBase = &bullet;

	Projectile slime;
	slime.gravity = 500.0;
	slime.explosionRadius = 1.0;
	//slime.setSpritesheet(&bulletSprites);
	slime.drawing = &redpix;
	Gun slimer;
	slimer.shootInterval = 0.01;
	slimer.bulletBase = &slime;

	Projectile grenade;
	grenade.gravity = 500.0;
	grenade.explosionRadius = 10;
	grenade.drawing = &redpix;
	Gun grenader;
	grenader.shootInterval = 1.0;
	grenader.bulletBase = &grenade;

	Gun* currentGun = &magnum;

	sf::Clock clock;
	bool quit = false;

	double timeFactor = 1.0;

	while (!quit)
	{
		sf::Time frameTime = clock.restart();
		frameTime = sf::microseconds(frameTime.asMicroseconds() * timeFactor);
		//cout << frameTime.asMilliseconds() << endl;
		//if (frameTime.asMilliseconds() > 100) frameTime = sf::milliseconds(100);	//at low framerates game becomes fps dependent to avoid collision errors etc.

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
				case sf::Keyboard::Num1:
				case sf::Keyboard::Numpad1:
				{
					currentGun = &magnum;
					break;
				}
				case sf::Keyboard::Num2:
				case sf::Keyboard::Numpad2:
				{
					currentGun = &slimer;
					break;
				}
				case sf::Keyboard::Num3:
				case sf::Keyboard::Numpad3:
				{
					currentGun = &grenader;
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
				break;
			}
			default: break;
			}
		}

		bool left = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		bool right = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		if (left || right)
		{
			sf::Vector2i pointA = window.win.mapCoordsToPixel(player.sprite.getPosition());
			sf::Vector2i pointB = sf::Mouse::getPosition(window.win);

			doubleVector velo = { double(pointB.x - pointA.x), double(pointB.y - pointA.y) };

			Projectile bullet = currentGun->shoot(player.bulletPosition(), velo, map);

			if (right) bullet.creative = true;
			projectiles.push_back(bullet);
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
			window.follow(player.position.x, player.position.y, sf::Vector2i(map.size), 128);
		}
		player.animate(frameTime.asSeconds());

		vector<list<Projectile>::iterator> deletions;
		for (auto i = projectiles.begin(); i != projectiles.end(); i++)
		{
			bool hit = i->inertiamove(frameTime.asSeconds(), map);

			if (hit)
			{
				sf::Color color = sf::Color::Transparent;
				if (i->creative) color = sf::Color::Green;

				if (i->explosionRadius >= 10.0)
				{
					map.circleExplosion(i->position.x, i->position.y, i->explosionRadius, sf::Color::Transparent);
					for (int j = 0; j < 24; j++)
					{
						Projectile b;
						b.gravity = 500.0;
						b.explosionRadius = 2.0;
						b.drawing = &redpix;
						b.position = i->position;
						b.creative = i->creative;
						b.launch(rand() % 200 - 100, rand() % 200 - 100);
						projectiles.push_back(b);
					}
				}
				else if (i->explosionRadius > 1.0) map.circleExplosion(i->position.x, i->position.y, i->explosionRadius, color);
				else if (color == sf::Color::Green) map.pixelExplosion((int)i->position.x, (int)i->position.y, color);
				else map.pixelExplosion(i->crashPixel.x, i->crashPixel.y, color);

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