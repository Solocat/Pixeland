#include "GameObject.h"

void GameObject::render(sf::RenderWindow& window)
{
	//sprite.setPosition((int)position.x, (int)position.y);
	sprite.setPosition(position.x, position.y);
	window.draw(sprite);
}

void GameObject::setSpritesheet(Spritesheet* sheet)
{
	sprites = sheet;
	sf::Sprite spr;
	spr.setTexture(sheet->fullTex);
	spr.setTextureRect(sheet->getRect(0));
	//spr.setOrigin(spr.getTextureRect().width / 2, spr.getTextureRect().height);
	//spr.setOrigin(1, 3);
	sprite = spr;
}

void GameObject::moveTo(double x, double y)
{
	position.x = x;
	position.y = y;
}