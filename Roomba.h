#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <cmath>
#include "Consts.h"
#include "Animation.h"

class Roomba{
	float x;
	float y;
	float xSpeed;
	float ySpeed;
	sf::Sprite sprite;
	sf::Texture texture;
	Animation animation;
	bool dead;
	unsigned char deathTimer;

public:
	Roomba();
	void draw(sf::RenderWindow& aWindow);
	void move(const Map& aMap, const unsigned aViewX, std::vector<Roomba>& aRoombas);
	void set_position(float x, float y);
	float get_x() const;
	float get_y() const;
	sf::FloatRect get_hit_box() const;
	void die();
};