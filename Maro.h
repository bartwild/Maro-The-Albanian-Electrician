#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class Maro{
	float x;
	float y;
	sf::Sprite sprite;
	sf::Texture texture;
public:
	Maro();
	void draw(sf::RenderWindow& i_window);
};

