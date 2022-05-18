#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <cmath>
#include <memory>
#include "Consts.h"
#include "Animation.h"
#include "Collisions.h"

class Roomba : public std::enable_shared_from_this <Roomba>{
	float x;
	float y;
	float xSpeed;
	float ySpeed;
	sf::Sprite sprite;
	std::shared_ptr<sf::Texture> texture;
	Animation walkAnimation;
	bool dead;
	unsigned char deathTimer;
	bool walkingOnRoomba;
	std::shared_ptr<Roomba> getRoomba() { return shared_from_this(); }
public:
	Roomba();
	void draw(sf::RenderWindow& aWindow);
	void update(const Map& aMap, const unsigned aViewX, std::vector< std::shared_ptr<Roomba>> aRoombas);
	void set_position(float x, float y);
	float get_x() const;
	float get_y() const;
	sf::FloatRect get_hit_box() const;
	void die(bool instant);
	unsigned char get_death_timer() const;
	bool get_whether_dead() const;
	float get_ySpeed() const;
	bool get_walkingOnRoomba() const;
};