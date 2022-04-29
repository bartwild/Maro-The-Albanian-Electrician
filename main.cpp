#include "Maro.h"
#include <array>
#include <chrono>
#include <cmath>
#include "Consts.h"


void draw_map(sf::RenderWindow& aWindow, const sf::Texture& aMapTexture, const Map& aMap) {
	sf::Sprite cell_sprite(aMapTexture);
	for (unsigned short i = 0; i < aMap.size(); i++) {
		for (unsigned short j = 0; j < aMap[i].size(); j++) {
			if (Cell::Empty == aMap[i][j]) continue;
			cell_sprite.setPosition(CELL_SIZE * i, CELL_SIZE * j);
			if (Cell::Wall == aMap[i][j]) {
				cell_sprite.setTextureRect(sf::IntRect(CELL_SIZE * 2, CELL_SIZE * 0, CELL_SIZE, CELL_SIZE));
				aWindow.draw(cell_sprite);
			}
		}
	}
}


int main() {
	Maro maro;
	sf::Event event;
	sf::Image map_sketch;
	map_sketch.loadFromFile("MapSketch.png");
	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE*SCREEN_WIDTH, SCREEN_RESIZE*SCREEN_HEIGHT), "Maro The Albanian Electrician", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));
	window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));
	sf::Texture mapTexture;
	unsigned view_x;
	mapTexture.loadFromFile("Map.png");
	Map map(SCREEN_WIDTH / CELL_SIZE);
	for (unsigned short i = 0; i < map.size(); i++) {
		for (unsigned short j = map[i].size()-2; j < map[i].size(); j++) {
			map[i][j] = Cell::Wall;
		}
	}
	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	while (window.isOpen()) {
		view_x = std::clamp<int>(round(maro.get_x()) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * map.size() - SCREEN_WIDTH);
		view.reset(sf::FloatRect(view_x, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
		window.setView(view);
		window.clear(sf::Color(0, 219, 255));
		draw_map(window, mapTexture, map);
		maro.draw(window);
		maro.move(map);
		window.display();
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
	return 0;
}