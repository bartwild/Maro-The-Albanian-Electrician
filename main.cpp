#include "Maro.h"
#include <array>
#include <chrono>
#include <cmath>
#include <algorithm>
#include "Consts.h"


void draw_background(unsigned short i, unsigned short j, unsigned short mapHeight, sf::Sprite& cellSprite, sf::Vector2u& mapSize, const sf::Image& aMapSketch, sf::RenderWindow& aWindow) {
	unsigned short x = 0;
	unsigned short y = 0;
	sf::Color pixel = aMapSketch.getPixel(i, j + 2 * mapHeight);
	sf::Color pixelDown = sf::Color(0, 0, 0, 0);
	sf::Color pixelLeft = sf::Color(0, 0, 0, 0);
	sf::Color pixelRight = sf::Color(0, 0, 0, 0);
	sf::Color pixelUp = sf::Color(0, 0, 0, 0);
	cellSprite.setPosition(CELL_SIZE * i, CELL_SIZE * j);
	if (pixel.a == 255) {
		if (i > 0) pixelLeft = aMapSketch.getPixel(i - 1, j + 2 * mapHeight);
		if (j > 0) pixelUp = aMapSketch.getPixel(i, j + 2 * mapHeight - 1);
		if (i < mapSize.x - 1) pixelRight = aMapSketch.getPixel(i + 1, j + 2 * mapHeight);
		if (j < mapHeight - 1) pixelDown = aMapSketch.getPixel(i, j + 2 * mapHeight + 1);
		if (pixel == sf::Color(255, 255, 255)) { // clouds
			x = 8;
			if (pixelUp == sf::Color(255, 255, 255)) {
				y = 1;
			}
			if (pixelLeft == sf::Color(255, 255, 255)) {
				if (pixelRight != sf::Color(255, 255, 255)) {
					x = 9;
				}
			}
			else if (pixelRight == sf::Color(255, 255, 255)) {
				x = 7;
			}
		}
		else if (sf::Color(146, 219, 0) == pixel) { // grass
			x = 5;
		}
		else if (sf::Color(146, 146, 0) == pixel) {
			x = 4;
		}
		else if (sf::Color(146, 182, 0) == pixel) {
			x = 6;
		}
		else if (pixel == sf::Color(0, 73, 0)) { // hills outline
			y = 1;
			if (pixelLeft == sf::Color(0, 109, 0)) {
				if (pixelRight != sf::Color(0, 109, 0)) {
					x = 2;
				}
			}
			else if (pixelRight == sf::Color(0, 109, 0)) {
				x = 1;
			}
		}
		else if (pixel == sf::Color(0, 109, 0)) { // hills
			y = 1;
			if (pixelLeft == sf::Color(0, 73, 0)) {
				x = 3;
			}
			else if (pixelRight == sf::Color(0, 73, 0)) {
				x = 5;
			}
			else {
				x = 4;
			}
		}
		else if (pixel == sf::Color(109, 255, 85)) { // flagpole
			x = 12;
			if (pixelLeft == sf::Color(109, 255, 85)) {
				y = 1;
			}
		}
		cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * x, CELL_SIZE * y, CELL_SIZE, CELL_SIZE));
		aWindow.draw(cellSprite);
	}
}


void draw_objects(unsigned short i, unsigned short j, sf::Sprite& cellSprite, const sf::Image& aMapSketch, sf::Sprite& questionBlock, sf::RenderWindow& aWindow, const Map& aMap) {
	unsigned short x = 0;
	unsigned short y = 0;
	cellSprite.setPosition(CELL_SIZE * i, CELL_SIZE * j);
	questionBlock.setPosition(CELL_SIZE * i, CELL_SIZE * j);
	if (aMap[i][j] == Cell::QuestionBlock) { // Question block
		questionBlock.setTextureRect(sf::IntRect(CELL_SIZE * 1, CELL_SIZE * 0, CELL_SIZE, CELL_SIZE));
		aWindow.draw(questionBlock);
	}
	else {
		if (aMap[i][j] == Cell::Pipe) {
			if (aMap[i][j - 1] == Cell::Pipe) {
				y = 1;
			}
			else {
				y = 0;
			}
			if (aMap[i - 1][j] == Cell::Pipe) {
				x = 11;
			}
			else {
				x = 10;
			}
		}
		else if (aMap[i][j] == Cell::Wall) {
			y = 0;

			if (aMapSketch.getPixel(i, j) == sf::Color(0, 0, 0)) { // Walls
				x = 2;
			}
			else { // Solid blocks
				x = 3;
			}

		}
		cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * x, CELL_SIZE * y, CELL_SIZE, CELL_SIZE));
		aWindow.draw(cellSprite);
	}
}


void draw_map(unsigned viewX, const sf::Image& aMapSketch, sf::RenderWindow& aWindow, const sf::Texture& aMapTexture, const sf::Texture& aQuestionBlock, const Map& aMap) {
	unsigned short mapEnd = ceil((viewX + SCREEN_WIDTH) / static_cast<float>(CELL_SIZE));
	unsigned short mapHeight = floor(aMapSketch.getSize().y / 3.f);
	unsigned short mapStart = floor(viewX / static_cast<float>(CELL_SIZE));
	sf::Sprite cellSprite(aMapTexture);
	sf::Sprite questionBlock(aQuestionBlock);
	sf::Vector2u mapSize = aMapSketch.getSize();
	for (unsigned short i = mapStart; i < mapEnd; i++) {
		for (unsigned short j = 0; j < mapHeight; j++) {
			draw_background(i, j, mapHeight, cellSprite, mapSize, aMapSketch, aWindow);
			if (aMap[i][j] != Cell::Empty)	draw_objects(i, j, cellSprite, aMapSketch, questionBlock, aWindow, aMap);
		}
	}
}



Map sketch_to_map(const sf::Image& aMapSketch, Maro& aMaro){
	sf::Vector2u mapSize = aMapSketch.getSize();
	Map finalMap(mapSize.x);
	for (unsigned short i = 0; i < mapSize.x; i++){
		for (unsigned short j = 0; j < 2 * floor(mapSize.y/3); j++){
			sf::Color pixel = aMapSketch.getPixel(i,j);
			if (j < floor(mapSize.y/3)){
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
				aMaro.set_position(CELL_SIZE*i, CELL_SIZE*(j-floor(mapSize.y/3)));
			}
		}
	}
	return finalMap;
}


void whole_Game() {
	Maro maro;
	unsigned viewX;
	sf::Event event;
	sf::Image mapSketch;
	sf::Texture mapTexture;
	sf::Texture questionBlock;
	std::chrono::microseconds lag(0);
	std::chrono::steady_clock::time_point previousTime;
	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Maro The Albanian Electrician", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));
	window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));
	questionBlock.loadFromFile("QuestionBlock.png");
	previousTime = std::chrono::steady_clock::now();
	mapSketch.loadFromFile("LevelSketch0.png");
	Map map = sketch_to_map(mapSketch, maro);
	mapTexture.loadFromFile("Map.png");
	while (window.isOpen()) {
		std::chrono::microseconds deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previousTime);
		lag += deltaTime;
		previousTime += deltaTime;
		while (FRAME_DURATION <= lag) {
			lag -= FRAME_DURATION;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed){
					window.close();
				}
			}
			maro.move(map);
			viewX = std::clamp<int>(round(maro.get_x()) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * map.size() - SCREEN_WIDTH);
			if (FRAME_DURATION > lag) {
				view.reset(sf::FloatRect(viewX, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
				window.setView(view);
				window.clear(sf::Color(0, 219, 255));
				draw_map(viewX, mapSketch, window, mapTexture, questionBlock, map);
				maro.draw(window);
				window.display();
			}
		}
	}
}


int main() {
	whole_Game();
	return 0;
}