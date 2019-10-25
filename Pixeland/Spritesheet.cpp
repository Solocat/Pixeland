#include "Spritesheet.h"

using namespace std;

Spritesheet::Spritesheet()
{
	tileWidth = 0;
	tileHeight = 0;
}

Spritesheet::Spritesheet(string _file, int _tileWidth, int _tileHeight, bool alpha)
{
	makeSheet(_file, _tileWidth, _tileHeight, alpha);
}

Spritesheet::~Spritesheet()
{

}

void Spritesheet::makeSheet(string _file, int _tileWidth, int _tileHeight, bool alpha)
{
	tileWidth = _tileWidth;
	tileHeight = _tileHeight;

	cout << "Loading " << _file.c_str() << "... ";

	if (alpha)
	{
		sf::Image img;
		if (!img.loadFromFile(_file))
		{
			cout << "Loading failed" << endl;
			return;
		}
		img.createMaskFromColor(sf::Color::Black);
		fullTex.loadFromImage(img);
	}
	else if (!fullTex.loadFromFile(_file))
	{
		cout << "Loading failed" << endl;
		return;
	}

	cout << "Ok" << endl;
}

sf::IntRect Spritesheet::getRect(unsigned i)
{
	sf::IntRect recto;
	recto.width = tileWidth;
	recto.height = tileHeight;

	unsigned pix = i * tileWidth;
	recto.left = pix % fullTex.getSize().x;
	recto.top = pix / fullTex.getSize().y;

	return recto;
}