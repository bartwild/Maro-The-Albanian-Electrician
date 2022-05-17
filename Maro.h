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
	unsigned char growthTimer;
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
	void reset();
	void become_small();
	void become_big();
	void draw(sf::RenderWindow& aWindow);
	void update(LevelManager& levelManager, unsigned int aViewX, Map& aMap, std::vector<std::shared_ptr<Roomba>> aRoombas);
	bool xMove(bool moving);
	void yMove(unsigned char yCollision);
	void set_position(float x, float y);
	float get_x() const;
	float get_y() const;
	void die(bool instant);
	char get_death_timer();
	void draw_mushrooms(const unsigned aViewX, sf::RenderWindow& aWindow);
	sf::FloatRect get_hit_box() const;
	void check_collision_with_Roombas(std::vector<std::shared_ptr<Roomba>> aRoombas);
	void check_collision_with_Mushrooms(std::vector<Mushroom>& aMushrooms);
};

