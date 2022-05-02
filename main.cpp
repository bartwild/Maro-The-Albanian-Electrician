#include "Maro.h"
#include <array>
#include <chrono>
#include <cmath>
#include <algorithm>
#include "Consts.h"


void draw_map(const sf::Image& aMapSketch, sf::RenderWindow& aWindow, const sf::Texture& aMapTexture, const Map& aMap) {
	sf::Sprite cellSprite(aMapTexture);
	sf::Vector2u mapSize = aMapSketch.getSize();
	unsigned short x = 0;
	unsigned short y = 0;
	sf::Color pixelDown = sf::Color(0, 0, 0, 0);
	sf::Color pixelLeft = sf::Color(0, 0, 0, 0);
	sf::Color pixelRight = sf::Color(0, 0, 0, 0);
	sf::Color pixelUp = sf::Color(0, 0, 0, 0);
	for (unsigned short i = 0; i < mapSize.x; i++) {
		for (unsigned short j = 0; j < mapSize.y; j++) {
			sf::Color pixel = aMapSketch.getPixel(i, j);
			if (255 == pixel.a) continue;
			if (i > 0 ) pixelLeft = aMapSketch.getPixel(i - 1, j);
			if (j > 0 ) pixelUp = aMapSketch.getPixel(i, j - 1);
			if (i < mapSize.x - 1 ) pixelRight = aMapSketch.getPixel(i + 1, j);
			if (j < floor (0.5f * mapSize.y-1) - 1 ) pixelDown = aMapSketch.getPixel(i, j + 1);
			cellSprite.setPosition(CELL_SIZE * i, CELL_SIZE * j);
			if (pixel == sf::Color(255, 255, 255)) {
				x = 8;
				if (pixelUp == sf::Color(255, 255, 255)) {
					y = 1;
				}
				if (pixelLeft == sf::Color(255, 255, 255)){
					if (pixelRight != sf::Color(255, 255, 255)){
						x = 9;
					}
				}
				else if (pixelRight == sf::Color(255, 255, 255)) {
					x = 7;
				}
			}
			else if (pixel == sf::Color(146, 219, 0)) {
				x = 5;
				if (pixelLeft == sf::Color(146, 219, 0)){
					if (pixelRight != sf::Color(146, 219, 0)){
						x = 6;
					}
				}
				else if (pixelRight == sf::Color(146, 219, 0)) {
					x = 4;
				}
			}
			else if (pixel == sf::Color(0, 73, 0)) {
				y = 1;
				if (pixelLeft == sf::Color(0, 109, 0)){
					if (pixelRight != sf::Color(0, 109, 0)){
						x = 2;
					}
				}
				else if (pixelRight == sf::Color(0, 109, 0)) {
					x = 1;
				}
			}
			else if (pixel == sf::Color(0, 109, 0)) {
				x = 4;
				y = 1;
				if (pixelLeft == sf::Color(0, 73, 0)){
					x = 3;
				}
				else if (pixelRight == sf::Color(0, 73, 0)) {
					x = 5;
				}
			}
			else if (pixel == sf::Color(109, 255, 85)) {
				x = 12;
				if (pixelLeft == sf::Color(109, 255, 85)){
					y = 1;
				}
			}
			cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * x, CELL_SIZE * y, CELL_SIZE, CELL_SIZE));
			aWindow.draw(cellSprite);
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
	return finalMap;
}


int main() {
	std::chrono::microseconds lag(0);
	std::chrono::steady_clock::time_point previousTime;
	Maro maro;
	sf::Event event;
	sf::Image mapSketch;
	mapSketch.loadFromFile("MapSketch.png");
	sketch_to_map(mapSketch, maro);
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
				draw_map(mapSketch, window, mapTexture, map);
				maro.draw(window);
				window.display();
			}
		}
	}
	return 0;
}