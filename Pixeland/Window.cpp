#include "Window.h"

Window::Window(string _title, int _width, int _height, int fps)
{
	mouseFocus = false;
	keyFocus = false;
	fps = 0;

	title = _title;

	win.create(sf::VideoMode(_width, _height), _title);
	if (fps > 0) win.setFramerateLimit(fps);

	camera = win.getDefaultView();
}

void Window::follow(int x, int y, sf::Vector2i bounds, unsigned margin) //player is offcenter
{
	sf::Vector2i size = (sf::Vector2i)camera.getSize();
	sf::Vector2i center = (sf::Vector2i)camera.getCenter();

	sf::Vector2i halfSize = size / 2;

	int xMin = x - margin;
	int xMax = x + margin;
	int yMin = y - margin;
	int yMax = y + margin;
	int xMaxMax = bounds.x - halfSize.x;
	int yMaxMax = bounds.y - halfSize.y;
	
	//way 1
	if (center.x < xMin) center.x = xMin;
	else if (center.x > xMax) center.x = xMax; 

	if (center.x < halfSize.x) center.x = halfSize.x;
	else if (center.x > xMaxMax) center.x = xMaxMax;

	//way 2
	/*xMin = max(xMin, halfSize.x);
	xMax = min(xMax, xMaxMax);
	if (xMin < xMax)
	{
		if (center.x < xMin) center.x = xMin;
		else if (center.x > xMax) center.x = xMax;
	}*/
	
	if (center.y < yMin) center.y = yMin;
	else if (center.y > yMax) center.y = yMax;

	if (center.y < halfSize.y) center.y = halfSize.y;
	else if (center.y > yMaxMax) center.y = yMaxMax;

	camera.setCenter(sf::Vector2f(center));
	win.setView(camera);
}

sf::IntRect Window::getArea() const
{
	sf::Vector2i size = (sf::Vector2i)camera.getSize();
	sf::Vector2i center = (sf::Vector2i)camera.getCenter();

	return sf::IntRect{
		center.x - (size.x / 2),
		center.y - (size.y / 2),
		size.x,
		size.y
	};
}