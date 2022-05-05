#pragma once
#include "Consts.h"
#include "Animation.h"

class Maro{
	bool flipped;
	bool onGround;
	bool dead;
	float x;
	float y;
	float xSpeed;
	float ySpeed;
	unsigned char deathTimer;
	unsigned char jumpTimer;
	Animation walkAnimation;
	sf::Sprite sprite;
	sf::Texture texture;
public:
	Maro();
	void draw(sf::RenderWindow& aWindow);
	void move(const Map& aMap);
	void set_position(float x, float y);
	float get_x() const;
	float get_y() const;
	void die();
	char get_death_timer();
	sf::FloatRect get_hit_box() const;
};

