#pragma once

class Gamemap 
{
public:
	Gamemap(string file) {
		img.loadFromFile(file);

		size = img.getSize();

		hardMask = std::vector<bool>(size.x * size.y);
		std::fill(hardMask.begin(), hardMask.end(), false);

		for (int i = 0; i < size.x * size.y; i++)
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
		if (x >= size.x || y >= size.y) cout << "out of map range";
		return hardMask[size.x * y + x];
	}

	sf::Image img;
	sf::Texture tex;
	sf::Sprite spr;
	sf::Vector2u size;
	std::vector<bool> hardMask;
};