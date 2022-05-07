#pragma once
#include "Consts.h"
#include "Maro.h"
#include "Roomba.h"

class Maro;


class LevelManager{
	Animation questionBlockAnimation;
	sf::Image mapSketch;
public:
	LevelManager(sf::Image& aMapSketch);
	sf::Color Object_to_color(std::string objectName);
	void update();
	void draw_map(const bool background, unsigned viewX, sf::RenderWindow& aWindow, const sf::Texture& aMapTexture, const sf::Texture& aQuestionBlock, const Map& aMap);
	void draw_objects(unsigned short i, unsigned short j, sf::Sprite& cellSprite, sf::RenderWindow& aWindow, const Map& aMap);
	void draw_background(unsigned short i, unsigned short j, unsigned short mapHeight, sf::Sprite& cellSprite, sf::Vector2u& mapSize, sf::RenderWindow& aWindow);
	Map sketch_to_map(Maro& aMaro, std::vector<Roomba>& aRoombas);
	void set_map_cell(Map& aMap, const unsigned short aX, const unsigned short aY, const Cell& aCell);
	sf::Color get_map_sketch_pixel(const unsigned short i_x, const unsigned short i_y) const;
};

