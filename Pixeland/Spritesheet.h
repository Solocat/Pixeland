#pragma once

#include <vector>
#include "Window.h"
//#include <SFML/Graphics.hpp>

//this contains the textures of all the possible tiles
class Spritesheet
{
public:
	Spritesheet();
	Spritesheet(string _file, int _tileWidth, int _tileHeight, bool alpha);
	~Spritesheet();
	void makeSheet(string _file, int _tileWidth, int _tileHeight, bool alpha);	//load image and chop it into tiles of requested size
	sf::IntRect getRect(unsigned i);
	//sf::Image rotateFrameCW(unsigned index, Window window);

	sf::Texture fullTex; //whole image file
	int tileWidth;
	int tileHeight;
};

