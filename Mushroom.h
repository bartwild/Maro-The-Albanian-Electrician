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
	char direction;
	sf::Sprite sprite;
	sf::Texture texture;
public:
	Mushroom(const float x, const float y);
	bool get_dead() const;
	void die(const bool isDead);
	void draw(const unsigned aViewX, sf::RenderWindow& aWindow);
	void move(const unsigned aViewX, const Map& aMap);
	sf::FloatRect get_hit_box() const;
};
