#pragma once
#include "Consts.h"

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
	Animation() {};
	Animation(const  unsigned short aWidth, const std::string& aTextureLocation, const unsigned short aSpeed);
	void draw(sf::RenderWindow& window);
	void set_flipped(bool value);
	void step(bool isBig);
	void set_position(const short i_x, const short i_y);
};

