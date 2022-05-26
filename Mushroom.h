#pragma once
#include "Consts.h"
#include "LevelManager.h"
#include "Collisions.h"

class Collisions;

class Mushroom {
	bool dead;
	float xSpeed;
	float ySpeed;
	float x;
	float y;
	float startY;
	sf::Sprite sprite;
	sf::Texture texture;
public:
	Mushroom(const float x, const float y);
	void die(const bool isDead);
	void draw(const unsigned aViewX, sf::RenderWindow& aWindow);
	void move(const unsigned aViewX, const Map& aMap);
	sf::FloatRect get_hit_box() const;
	bool get_whether_dead() const;
	float get_x() const;
	float get_y() const;
	float get_xSpeed() const;
	float get_ySpeed() const;
	void set_x_y(float newX, float newY);
};
