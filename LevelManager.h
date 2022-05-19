#pragma once
#include "Consts.h"
#include "Maro.h"
#include "Roomba.h"

class Maro;


class LevelManager{
	Animation questionBlockAnimation;
	Animation coinAnimation;
	sf::Image mapSketch;
	sf::Texture pole;
	std::vector<Object> questionBlockCoins;
public:
	LevelManager(sf::Image& aMapSketch);
	void update();
	void set_sketch(const sf::Image& sketch);
	void draw_map(const bool background, unsigned viewX, sf::RenderWindow& aWindow, sf::Color& backgroundColor, const sf::Texture& aMapTexture, const Map& aMap);
	void draw_objects(const bool underground, unsigned short i, unsigned short j, sf::Sprite& cellSprite, sf::RenderWindow& aWindow, const Map& aMap);
	void draw_background(const bool underground, unsigned short i, unsigned short j, unsigned short mapHeight, sf::Sprite& cellSprite, sf::Vector2u& mapSize, sf::RenderWindow& aWindow);
	Map sketch_to_map(Maro& aMaro, unsigned char& finish, sf::Color& backgroundColor, std::vector<std::shared_ptr<Roomba>>& aRoombas);
	void set_map_cell(Map& aMap, const unsigned short aX, const unsigned short aY, const Cell& aCell);
	sf::Color get_map_sketch_pixel(const unsigned short i_x, const unsigned short i_y) const;
	void add_question_block_coin(const unsigned short x, const unsigned short y);
};

