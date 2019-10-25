#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

//each window has its own renderer and event handler for window-related events
class Window
{
public:
	Window(string _title, int _width, int _height, int fps);
	void follow(int x, int y, sf::Vector2i bounds, unsigned margin);

	sf::IntRect getArea() const;

	string title;

	sf::RenderWindow win;
	sf::View camera;
	int fps;

	bool mouseFocus;
	bool keyFocus;
};

