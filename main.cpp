#include "Maro.h"
#include <array>
#include <chrono>
#include <cmath>
#include <algorithm>
#include "Consts.h"


void draw_map(sf::RenderWindow& aWindow, const sf::Texture& aMapTexture, const Map& aMap) {
	sf::Sprite cellSprite(aMapTexture);
	for (unsigned short i = 0; i < aMap.size(); i++) {
		for (unsigned short j = 0; j < aMap[i].size(); j++) {
			if (Cell::Empty == aMap[i][j]) continue;
			cellSprite.setPosition(CELL_SIZE * i, CELL_SIZE * j);
			if (Cell::Wall == aMap[i][j]) {
				cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * 2, CELL_SIZE * 0, CELL_SIZE, CELL_SIZE));
				aWindow.draw(cellSprite);
			}
		}
	}
}

Map sketch_to_map(const sf::Image& aMapSketch, Maro& aMaro){
	sf::Vector2u mapSize = aMapSketch.getSize();
	Map finalMap(mapSize.x);
	for (unsigned short i = 0; i < mapSize.x; i++){
		for (unsigned short j = 0; j < mapSize.y; j++){
			sf::Color pixel = aMapSketch.getPixel(i,j);
			if (j < floor(0.5f * mapSize.y)){
				if (pixel == sf::Color(182, 73, 0)){
					finalMap[i][j] = Cell::Brick;
				}
				else if (pixel == sf::Color(0, 182, 0) || pixel == sf::Color(0, 146, 0) || pixel == sf::Color(0, 219, 0)){
					finalMap[i][j] = Cell::Pipe;
				}
				else if (pixel == sf::Color(255, 146, 85) || pixel == sf::Color(255, 73, 85)){
					finalMap[i][j] = Cell::QuestionBlock;
				}
				else if (pixel == sf::Color(0, 0, 0) || pixel == sf::Color(146, 73, 0)){
					finalMap[i][j] = Cell::Wall;
				}
				else{
					finalMap[i][j] = Cell::Empty;
				}
			}
			else if(pixel == sf::Color(255, 0, 0)){
				aMaro.set_position(CELL_SIZE*i, CELL_SIZE*(j-floor(0.5f*mapSize.y)));
			}
		}
	}
}


int main() {
	std::chrono::microseconds lag(0);
	std::chrono::steady_clock::time_point previousTime;
	Maro maro;
	sf::Event event;
	sf::Image mapSketch;
	mapSketch.loadFromFile("MapSketch.png");
	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE*SCREEN_WIDTH, SCREEN_RESIZE*SCREEN_HEIGHT), "Maro The Albanian Electrician", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));
	window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));
	sf::Texture mapTexture;
	unsigned viewX;
	mapTexture.loadFromFile("Map.png");
	Map map(SCREEN_WIDTH / CELL_SIZE);
	for (unsigned short i = 0; i < map.size(); i++) {
		for (unsigned short j = map[i].size() - 2; j < map[i].size(); j++) {
			map[i][j] = Cell::Wall;
		}
	}
	map[12][11] = Cell::Wall;
	map[6][11] = Cell::Wall;
	map[12][12] = Cell::Wall;
	map[6][12] = Cell::Wall;
	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	previousTime = std::chrono::steady_clock::now();
	while (window.isOpen()) {
		std::chrono::microseconds deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previousTime);
		lag += deltaTime;
		previousTime += deltaTime;
		while (FRAME_DURATION <= lag) {
			lag -= FRAME_DURATION;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}
			maro.move(map);
			viewX = std::clamp<int>(round(maro.get_x()) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * map.size() - SCREEN_WIDTH);
			if (FRAME_DURATION > lag) {
				view.reset(sf::FloatRect(viewX, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
				window.setView(view);
				window.clear(sf::Color(0, 219, 255));
				draw_map(window, mapTexture, map);
				maro.draw(window);
				window.display();
			}
		}
	}
	return 0;
}