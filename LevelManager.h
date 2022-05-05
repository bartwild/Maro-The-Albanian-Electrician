#pragma once
#include "Consts.h"
#include "Maro.h"
#include "Roomba.h"


class LevelManager{
public:
	static sf::Color Object_to_color(std::string objectName);
	static void draw_map(unsigned viewX, const sf::Image& aMapSketch, sf::RenderWindow& aWindow, const sf::Texture& aMapTexture, const sf::Texture& aQuestionBlock, const Map& aMap);
	static void draw_objects(unsigned short i, unsigned short j, sf::Sprite& cellSprite, const sf::Image& aMapSketch, sf::Sprite& questionBlock, sf::RenderWindow& aWindow, const Map& aMap);
	static void draw_background(unsigned short i, unsigned short j, unsigned short mapHeight, sf::Sprite& cellSprite, sf::Vector2u& mapSize, const sf::Image& aMapSketch, sf::RenderWindow& aWindow);
	static Map sketch_to_map(const sf::Image& aMapSketch, Maro& aMaro, std::vector<Roomba>& aRoombas);
};

