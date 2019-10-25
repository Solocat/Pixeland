#include <SFML/Graphics.hpp>
#include "Character.h"
#include <array>

int main()
{
	const int resX = 1024;
	const int resY = 512;
	sf::RenderWindow window;
	window.create(sf::VideoMode(resX, resY), "Pixeland");
	window.setFramerateLimit(60);
	sf::View view = window.getDefaultView();
	view.setCenter(200, 200);
	view.zoom(0.25);
	window.setView(view);

	sf::Image img;
	img.loadFromFile("dirtmap.png");
	
	std::array<bool, resX * resY> hardMask;

	for (int i = 0; i < resX * resY; i++)
	{
		if (img.getPixel(i % resX, i / resX) == sf::Color::Magenta)
		{
			hardMask[i] = false;
		}
		else hardMask[i] = true;
	}
	img.createMaskFromColor(sf::Color::Magenta);

	sf::Texture tex;
	tex.loadFromImage(img);
	sf::Sprite map;
	map.setTexture(tex);

	/*
	Character player;
	player.gravity = 3000.0;
	player.runSpeed = 10 * TileRes;
	player.jumpVelocity = 22 * TileRes;
	player.jumpTimeMax = 0.14;
	player.terminalVelocity = 22 * TileRes;

	player.setHitbox(TileRes, 52);

	Spritesheet playerSprites("runnyC.png", 48, 64, true);
	player.setSpritesheet(&playerSprites);

	player.anims[AnimState::IDLE] = Animation(7, 0, 0.0);
	player.anims[AnimState::MOVE] = Animation(0, 8, 0.01875);
	player.anims[AnimState::JUMP] = Animation(1, 0, 0.0);
	player.anims[AnimState::FALL] = Animation(6, 0, 0.0);
	player.changeAnim(AnimState::IDLE);
	
	player.moveTo(112, SCREEN_HEIGHT - 128);
	*/
	
	

	sf::RectangleShape player;
	player.setFillColor(sf::Color::Red);
	player.setSize(sf::Vector2f(1, 1));
	//player.setOrigin(0.5, 0.5);
	player.setPosition(200, 200);
	float speed = 0.5;

	bool quit = false;
	while (!quit)
	{
		sf::Event e;
		while (window.pollEvent(e))
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

				default: break;
				}
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{ 
			player.move(-speed, 0); 
			sf::Vector2i pos = (sf::Vector2i)player.getPosition();
			if (hardMask[pos.y * resX + pos.x]) 
			{
				pos.x++;
				player.setPosition((sf::Vector2f)pos);
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
		{ 
			player.move(speed, 0); 
			sf::Vector2i pos = (sf::Vector2i)player.getPosition();
			pos.x++;
			if (hardMask[pos.y * resX + pos.x])
			{
				pos.x--;
				player.setPosition((sf::Vector2f)pos);
			}
		}

		player.move(0, 0.2f);

		sf::Vector2i pos = (sf::Vector2i)player.getPosition();
		pos.y++;
		if (hardMask[pos.y * resX + pos.x])
		{
			pos.y--;
			player.setPosition((sf::Vector2f)pos);
		}

		view.setCenter(player.getPosition());
		window.setView(view);

		window.clear();
		window.draw(map);
		window.draw(player);
		window.display();
	}
}