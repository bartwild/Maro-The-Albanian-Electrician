#pragma once
#include "Consts.h"
#include "LevelManager.h"
#include "Collisions.h"
#include "Character.h"

class Collisions;

class Mushroom: public Character {
	float startY;
public:
	Mushroom(const float x, const float y);
	void die(const bool isDead);
	void draw(const unsigned aViewX, sf::RenderWindow& aWindow);
	void move(const unsigned aViewX, const Map& aMap);
	bool get_whether_dead() const;
};
