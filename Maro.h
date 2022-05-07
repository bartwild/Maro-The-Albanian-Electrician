#pragma once
#include "Consts.h"
#include "Animation.h"
#include "Mushroom.h"
#include "LevelManager.h"

class Mushroom;
class LevelManager;

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
	std::vector<Mushroom> mushrooms;
public:
	Maro();
	void draw(sf::RenderWindow& aWindow);
	void move(LevelManager& levelManager, unsigned int aViewX, Map& aMap);
	void set_position(float x, float y);
	float get_x() const;
	float get_y() const;
	void die();
	char get_death_timer();
	void draw_mushrooms(const unsigned aViewX, sf::RenderWindow& aWindow);
	sf::FloatRect get_hit_box() const;
};

