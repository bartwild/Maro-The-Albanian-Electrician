#pragma once
#include <memory>
#include "Consts.h"
#include "Animation.h"
#include "Mushroom.h"
#include "LevelManager.h"
#include "Roomba.h"

class Mushroom;
class LevelManager;

class Maro{
	bool flipped;
	bool onGround;
	bool dead;
	bool big;
	float x;
	float y;
	float xSpeed;
	float ySpeed;
	unsigned char deathTimer;
	unsigned char jumpTimer;
	unsigned char hitTimer;
	bool hit;
	Animation walkAnimation;
	Animation bigWalkAnimation;
	sf::Sprite sprite;
	sf::Texture texture;
	std::vector<Mushroom> mushrooms;
public:
	Maro();
	void become_small();
	void become_big();
	void draw(sf::RenderWindow& aWindow);
	void move(LevelManager& levelManager, unsigned int aViewX, Map& aMap, std::vector<Roomba>& aRoombas);
	void set_position(float x, float y);
	float get_x() const;
	float get_y() const;
	void die(bool instant);
	char get_death_timer();
	void draw_mushrooms(const unsigned aViewX, sf::RenderWindow& aWindow);
	sf::FloatRect get_hit_box() const;
};

