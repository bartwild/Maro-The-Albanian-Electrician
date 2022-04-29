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
				continue;
			}
		}
	}
}

int main() {
	Maro maro;
	sf::Color backgroundColor = sf::Color(0, 210, 255);
	sf::Event event;
	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE*SCREEN_WIDTH, SCREEN_RESIZE*SCREEN_HEIGHT), "Maro The Albanian Electrician", sf::Style::Close);
	sf::Texture mapTexture;
	mapTexture.loadFromFile("Map.png");
	Map map(SCREEN_WIDTH / CELL_SIZE);
	for (unsigned short i = 0; i < map.size(); i++) {
		for (unsigned short j = map[i].size()-2; j < map[i].size(); j++) {
			map[i][j] = Cell::Wall;
		}
	}
	while (window.isOpen()) {
		window.clear(backgroundColor);
		maro.draw(window);
		maro.update();
		window.display();
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
	return 0;
}