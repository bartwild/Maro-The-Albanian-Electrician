#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <cmath>
#include <memory>
#include "Consts.h"
#include "Animation.h"
#include "Collisions.h"
#include "Character.h"

class Roomba : public std::enable_shared_from_this <Roomba>, public Character{
	std::shared_ptr<sf::Texture> texture;
	Animation walkAnimation;
	unsigned char deathTimer;
	bool walkingOnRoomba;
	std::shared_ptr<Roomba> getRoomba();
public:
	Roomba();
	void draw(sf::RenderWindow& aWindow);
	void update(const Map& aMap, const unsigned aViewX, std::vector< std::shared_ptr<Roomba>>& aRoombas);
	void die(bool instant);
	unsigned char get_death_timer() const;
	bool get_whether_dead() const;
	bool get_walkingOnRoomba() const;
};