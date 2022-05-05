#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Animation{
	bool flipped;
	unsigned short iterator;
	unsigned short speed;
	unsigned short frame;
	unsigned short width;
	unsigned short totalFrames;
	sf::Sprite sprite;
	sf::Texture texture;
public:
	Animation(unsigned short aSpeed, unsigned short aWidth, const std::string& aTextureLocation);
	void draw(short x, short y, sf::RenderWindow& window);
	void set_flipped(bool value);
	void step();
};

