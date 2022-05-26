#include "LevelManager.h"


LevelManager::LevelManager(sf::Image& aMapSketch) {
	questionBlockAnimation = Animation(CELL_SIZE, "QuestionBlock.png", QUESTION_BLOCK_ANIMATION_SPEED);
	coinAnimation = Animation(CELL_SIZE, "Coin.png", QUESTION_BLOCK_ANIMATION_SPEED/2);
	pole.loadFromFile(TEXTURES_PATH + "PhonePole.png");
	mapSketch = aMapSketch;
}


void LevelManager::draw_background(const bool underground, unsigned short i, unsigned short j, unsigned short mapHeight, sf::Sprite& cellSprite, sf::Vector2u& mapSize, sf::RenderWindow& aWindow) {
	unsigned short x = 0;
	unsigned short y = 0;
	unsigned short background_j = j + 2 * mapHeight;
	sf::Color pixel = mapSketch.getPixel(i, background_j);
	sf::Color pixelDown = sf::Color(0, 0, 0, 0);
	sf::Color pixelLeft = sf::Color(0, 0, 0, 0);
	sf::Color pixelRight = sf::Color(0, 0, 0, 0);
	sf::Color pixelUp = sf::Color(0, 0, 0, 0);
	cellSprite.setPosition(CELL_SIZE * i, CELL_SIZE * j);
	if (pixel.a == 255) {
		if (i > 0) {
			pixelLeft = mapSketch.getPixel(i - 1, background_j);
		}
		if (j > 0) {
			pixelUp = mapSketch.getPixel(i, background_j - 1);
		}
		if (i < mapSize.x - 1) {
			pixelRight = mapSketch.getPixel(i + 1, background_j);
		}
		if (j < mapHeight - 1) {
			pixelDown = mapSketch.getPixel(i, background_j + 1);
		}
		if (pixel == sf::Color(255, 255, 255)) {
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
			if (pixelUp == sf::Color(109, 255, 85)) {
				y = 1;
			}
		}
		cellSprite.setTextureRect(sf::IntRect(CELL_SIZE * x, CELL_SIZE * y, CELL_SIZE, CELL_SIZE));
		aWindow.draw(cellSprite);
	}
}


void LevelManager::draw_objects(const bool underground, unsigned short i, unsigned short j, sf::Sprite& cellSprite, sf::RenderWindow& aWindow, const Map& aMap) {
	if (aMap[i][j] != Cell::Empty) {
		unsigned short x = 0;
		unsigned short y = 0;
		sf::Sprite cellSpritePole(pole);
		cellSprite.setPosition(CELL_SIZE * i, CELL_SIZE * j);
		cellSpritePole.setPosition(CELL_SIZE * i, CELL_SIZE * j);
		if (aMap[i][j] == Cell::Coin) {
			coinAnimation.set_position(CELL_SIZE * i, CELL_SIZE * j);
			coinAnimation.draw(aWindow);
		}
		else if (aMap[i][j] == Cell::QuestionBlock) {
			questionBlockAnimation.set_position(CELL_SIZE * i, CELL_SIZE * j);
			questionBlockAnimation.draw(aWindow);
		}
		else {
			y = 2 * underground;
			if (Cell::ActivatedQuestionBlock == aMap[i][j]) {
				x = 6;
				y++;
			}
			else if (aMap[i][j] == Cell::Pipe) {
				if (underground) {
					if (mapSketch.getPixel(i, j) == sf::Color(0, 182, 0)) {
						y = 1;
						if (aMap[i - 1][j] == Cell::Pipe) {
							x = 11;
						}
						else {
							x = 10;
						}
					}
					else if (mapSketch.getPixel(i, j) == (sf::Color(0, 146, 0))) {
						y = 0;
						if (mapSketch.getPixel(i - 1, j) == sf::Color(0, 146, 0)) {
							x = 11;
						}
						else if (mapSketch.getPixel(i + 1, j) == sf::Color(0, 146, 0)) {
							x = 10;
						}
						else {
							x = 10;
							if (mapSketch.getPixel(i, j - 1) == sf::Color(0, 146, 0)) {
								y = 3;
							}
							else {
								y = 2;
							}
						}
					}
					else if (mapSketch.getPixel(i, j) == (sf::Color(0, 219, 0)))
					{
						if (mapSketch.getPixel(1 + i, j) == sf::Color(0, 182, 0)) {
							x = 12;
						}
						else {
							x = 11;
						}
						if (mapSketch.getPixel(i, j - 1) == sf::Color(0, 219, 0)) {
							y = 3;
						}
						else {
							y = 2;
						}
					}
				}
				else if (!underground) {
					if (aMap[i][j - 3] == Cell::Pipe || aMap[i][j+1] != Cell::Pipe) {
						y = 3;
					}
					else if (aMap[i][j - 2] == Cell::Pipe) {
						y = 2;
					}
					else if (aMap[i][j - 1] == Cell::Pipe) {
						y = 1;
					}
					else {
						y = 0;
					}
					if (aMap[i - 1][j] == Cell::Pipe) {
						x = 1;
					}
					else {
						x = 0;
					}
					cellSpritePole.setTextureRect(sf::IntRect(CELL_SIZE * x, CELL_SIZE * y, CELL_SIZE, CELL_SIZE));
					aWindow.draw(cellSpritePole);
					return;
				}
			}
			else if (aMap[i][j] == Cell::Wall) {
				if (mapSketch.getPixel(i, j) == sf::Color(0, 0, 0)) { // Walls
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
}


void LevelManager::draw_map(const bool background, unsigned viewX, sf::RenderWindow& aWindow, sf::Color& backgroundColor, const sf::Texture& aMapTexture, const Map& aMap) {
	unsigned short mapEnd = ceil((viewX + SCREEN_WIDTH) / static_cast<float>(CELL_SIZE));
	unsigned short mapHeight = floor(mapSketch.getSize().y / 3.f);
	unsigned short mapStart = floor(viewX / static_cast<float>(CELL_SIZE));
	bool underground = 1;
	if (backgroundColor == sf::Color(0, 219, 255)) {
		underground = 0;
	}
	sf::Sprite cellSprite(aMapTexture);
	sf::Vector2u mapSize = mapSketch.getSize();
	for (unsigned short i = mapStart; i < mapEnd; i++) {
		for (unsigned short j = 0; j < mapHeight; j++) {
			if (!background) {
				for (const Object& questionBlockCoin : questionBlockCoins) {
					coinAnimation.set_position(questionBlockCoin.x, questionBlockCoin.y);
					coinAnimation.draw(aWindow);
				}
				draw_objects(underground, i, j, cellSprite, aWindow, aMap);
			}
			else {
				draw_background(underground, i, j, mapHeight, cellSprite, mapSize, aWindow);
			}
		}
	}
}


Map LevelManager::sketch_to_map(Maro& aMaro, unsigned char& finish, sf::Color& backgroundColor, std::vector<std::shared_ptr<Roomba>>& aRoombas) {
	sf::Vector2u mapSize = mapSketch.getSize();
	Map finalMap(mapSize.x);
	backgroundColor = mapSketch.getPixel(0, mapSize.y - 1);
	for (unsigned short i = 0; i < mapSize.x; i++) {
		for (unsigned short j = 0; j < 2 * floor(mapSize.y / 3); j++) {
			sf::Color pixel = mapSketch.getPixel(i, j);
			if (j < floor(mapSize.y / 3)) {
				if (pixel == sf::Color(182, 73, 0)) {
					finalMap[i][j] = Cell::Brick;
				}
				else if (pixel == sf::Color(0, 182, 0) || pixel == sf::Color(0, 146, 0) || pixel == sf::Color(0, 219, 0)) {
					finalMap[i][j] = Cell::Pipe;
				}
				else if (pixel == sf::Color(255, 255, 0)){
					finalMap[i][j] = Cell::Coin;
				}
				else if (pixel == sf::Color(255, 146, 85) || pixel == sf::Color(255, 73, 85)) {
					finalMap[i][j] = Cell::QuestionBlock;
				}
				else if (pixel == sf::Color(0, 0, 0) || pixel == sf::Color(146, 73, 0)) {
					finalMap[i][j] = Cell::Wall;
				}
				else {
					finalMap[i][j] = Cell::Empty;
					if (sf::Color(0, 255, 255) == pixel){
						finish = i;
					}
				}
			}
			else {
				if (pixel == sf::Color(255, 0, 0)) {
					aMaro.set_position(CELL_SIZE * i, CELL_SIZE * (j - floor(mapSize.y / 3)));
				}
				else if (pixel == (sf::Color(182, 73, 0))) {
					std::shared_ptr<Roomba> roomba = std::make_shared<Roomba>();
					roomba->set_position(CELL_SIZE * i, CELL_SIZE * (j - floor(mapSize.y / 3)));
					aRoombas.push_back(roomba);
				}
			}
		}
	}
	return finalMap;
}


void LevelManager::update() {
	for (Object& questionBlockCoin : questionBlockCoins) {
		questionBlockCoin.ySpeed += GRAVITY;
		questionBlockCoin.y += questionBlockCoin.ySpeed;
	}
	questionBlockCoins.erase(remove_if(questionBlockCoins.begin(), questionBlockCoins.end(), [](const Object& questionBlockCoin)
		{
			return 0 <= questionBlockCoin.ySpeed;
		}), questionBlockCoins.end());
	questionBlockAnimation.step(0);
	coinAnimation.step(1);
}


void LevelManager::set_sketch(const sf::Image& sketch) {
	mapSketch = sketch;
}


void LevelManager::set_map_cell(Map& aMap, unsigned short x, unsigned short y, const Cell& aCell) {
	aMap[x][y] = aCell;
}


sf::Color LevelManager::get_map_sketch_pixel(const unsigned short x, const unsigned short y) const {
	return mapSketch.getPixel(x, y);
}


void LevelManager::add_question_block_coin(const unsigned short x, const unsigned short y) {
	questionBlockCoins.push_back(Object(x, y, 0, COIN_JUMP_SPEED));
}