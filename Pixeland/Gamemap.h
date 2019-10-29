#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class Gamemap 
{
public:
	Gamemap(string file) {
		img.loadFromFile(file);

		size = img.getSize();

		hardMask = std::vector<bool>(size.x * size.y);
		std::fill(hardMask.begin(), hardMask.end(), false);

		for (unsigned i = 0; i < size.x * size.y; i++)
		{
			if (img.getPixel(i % size.x, i / size.x) != sf::Color::Magenta)
			{
				hardMask[i] = true;
			}
		}
		hardMask.shrink_to_fit();

		img.createMaskFromColor(sf::Color::Magenta);
		tex.loadFromImage(img);	
		spr.setTexture(tex);
	}

	bool isPixelHard(unsigned x, unsigned y) const
	{
		if (x >= size.x || y >= size.y)
		{
			cout << "out of map range"; 
			return true;
		}
			
		return hardMask[size.x * y + x];
	}

	bool checkCollision(double x, double y) const
	{
		if (x < 0.0 || y < 0.0 || x >= size.x || y >= size.y)
		{
			cout << "out of map range"; 
			return true;
		}
		return isPixelHard((unsigned)x, (unsigned)y);
	}

	void destroyPixel(unsigned x, unsigned y)
	{
		hardMask[size.x * y + x] = false;
		img.setPixel(x, y, sf::Color::Magenta);
	}

	void circleExplosion(double x, double y, double r)
	{
		sf::IntRect bounds;
		int leftBound = max(int(x - r+1), 0);
		int topBound = max(int(y - r+1), 0);
		int rightBound = min(int(x + r+1), (int)size.x - 1);
		int bottomBound = min(int(y + r+1), (int)size.y - 1);

		for (int j = topBound; j < bottomBound; j++)
		{
			for (int i = leftBound; i < rightBound; i++)
			{
				double dist = sqrt(
					(x-i)*(x-i) + (y-j)*(y-j)
				);
				if (dist <= r)
				{				
					destroyPixel(i, j);				
				}
			}
		}

		//img.createMaskFromColor(sf::Color::Magenta);
		//tex.update(pixels, rightBound-leftBound, bottomBound-topBound, leftBound, topBound);
		//tex.update(img);
		//const sf::Uint8* pixels = img.getPixelsPtr();
		tex.update(img.getPixelsPtr());
	}

	sf::Image img;
	sf::Texture tex;
	sf::Sprite spr;
	sf::Vector2u size;
	std::vector<bool> hardMask;
};