#include <SFML/Graphics.hpp>
#include "Character.h"
#include <vector>

int main()
{
	const int resX = 1024;
	const int resY = 512;
	Window window("Platform", resX, resY, 60);
	window.camera.zoom(0.25);
	
	Gamemap map("dirtmap.png");
	
	Character player;
	player.gravity = 1500.0;
	player.runSpeed = 10 * 8;
	player.jumpVelocity = 22 * 8;
	player.jumpTimeMax = 0.14;
	player.terminalVelocity = 22 * 8;

	player.setHitbox(1, 1);

	Spritesheet playerSprites("soldier.png", 3, 4, true);
	player.setSpritesheet(&playerSprites);

	player.anims[AnimState::IDLE] = Animation(0, 0, 0.0);
	player.anims[AnimState::MOVE] = Animation(0, 0, 0.01875);
	player.anims[AnimState::JUMP] = Animation(0, 0, 0.0);
	player.anims[AnimState::FALL] = Animation(0, 0, 0.0);
	player.changeAnim(AnimState::IDLE);
	
	player.moveTo(200, 200);

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

				player.shoot();
				break;
			}
			default: break;
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
			window.follow((int)player.position.x, (int)player.position.y, sf::Vector2i(resX, resY), 4*8);
		}
		player.animate(frameTime.asSeconds());

		window.win.clear();
		window.win.draw(map.spr);
		player.render(window.win);
		window.win.display();
	}
}