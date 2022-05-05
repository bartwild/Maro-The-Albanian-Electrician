#include "LevelManager.h"


void LevelManager::draw_background(unsigned short i, unsigned short j, unsigned short mapHeight, sf::Sprite& cellSprite, sf::Vector2u& mapSize, const sf::Image& aMapSketch, sf::RenderWindow& aWindow) {
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
		if (pixel == LevelManager::Object_to_color("clouds")) {
			x = 8;
			if (pixelUp == LevelManager::Object_to_color("clouds")) {
				y = 1;
			}
			if (pixelLeft == LevelManager::Object_to_color("clouds")) {
				if (pixelRight != LevelManager::Object_to_color("clouds")) {
					x = 9;
				}
			}
			else if (pixelRight == LevelManager::Object_to_color("clouds")) {
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
			if (pixelUp == sf::Color(109, 255, 85)) {
				y = 1;
			}
		}
		cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * x, CELL_SIZE * y, CELL_SIZE, CELL_SIZE));
		aWindow.draw(cellSprite);
	}
}


void LevelManager::draw_objects(unsigned short i, unsigned short j, sf::Sprite& cellSprite, const sf::Image& aMapSketch, sf::Sprite& questionBlock, sf::RenderWindow& aWindow, const Map& aMap) {
	unsigned short x = 0;
	unsigned short y = 0;
	cellSprite.setPosition(CELL_SIZE * i, CELL_SIZE * j);
	questionBlock.setPosition(CELL_SIZE * i, CELL_SIZE * j);
	if (aMap[i][j] == Cell::QuestionBlock) {
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




sf::Color LevelManager::Object_to_color(std::string objectName){
	if (objectName == "clouds") {
		return sf::Color(255, 255, 255);
	}
	else if (objectName == "grass"){
	}

}



void LevelManager::draw_map(unsigned viewX, const sf::Image& aMapSketch, sf::RenderWindow& aWindow, const sf::Texture& aMapTexture, const sf::Texture& aQuestionBlock, const Map& aMap) {
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



Map LevelManager::sketch_to_map(const sf::Image& aMapSketch, Maro& aMaro, std::vector<Roomba>& aRoombas) {
	sf::Vector2u mapSize = aMapSketch.getSize();
	Map finalMap(mapSize.x);
	for (unsigned short i = 0; i < mapSize.x; i++) {
		for (unsigned short j = 0; j < 2 * floor(mapSize.y / 3); j++) {
			sf::Color pixel = aMapSketch.getPixel(i, j);
			if (j < floor(mapSize.y / 3)) {
				if (pixel == sf::Color(182, 73, 0)) {
					finalMap[i][j] = Cell::Brick;
				}
				else if (pixel == sf::Color(0, 182, 0) || pixel == sf::Color(0, 146, 0) || pixel == sf::Color(0, 219, 0)) {
					finalMap[i][j] = Cell::Pipe;
				}
				else if (pixel == sf::Color(255, 146, 85) || pixel == sf::Color(255, 73, 85)) {
					finalMap[i][j] = Cell::QuestionBlock;
				}
				else if (pixel == sf::Color(0, 0, 0) || pixel == sf::Color(146, 73, 0)) {
					finalMap[i][j] = Cell::Wall;
				}
				else {
					finalMap[i][j] = Cell::Empty;
				}
			}
			else {
				if (pixel == sf::Color(255, 0, 0)) {
					aMaro.set_position(CELL_SIZE * i, CELL_SIZE * (j - floor(mapSize.y / 3)));
				}
				else if (pixel == (sf::Color(182, 73, 0))) {
					Roomba roomba;
					roomba.set_position(CELL_SIZE * i, CELL_SIZE * (j - floor(mapSize.y / 3)));
					aRoombas.push_back(roomba);
				}
			}
		}
	}
	return finalMap;
}
