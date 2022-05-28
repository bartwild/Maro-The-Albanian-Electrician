#pragma once
#include "Consts.h"


class Character {
protected:
	float x;
	float y;
	float xSpeed;
	float ySpeed;
	bool dead;
	sf::Sprite sprite;
	sf::Texture texture;
public:
	virtual float get_x() const;
	virtual float get_y() const;
	virtual float get_xSpeed() const;
	virtual float get_ySpeed() const;
	virtual void set_position(float newX, float newY);
	virtual sf::FloatRect get_hit_box() const;


};

