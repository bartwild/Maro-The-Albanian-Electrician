#include "Collisions.h"


void Collisions::get_x_y(float& x, float& y, unsigned char i, float cellX, float cellY, bool isBig) {
	switch (i) {
		case 0: {
			x = floor(cellX);
			y = floor(cellY);
			break;
		}
		case 1: {
			x = ceil(cellX);
			y = floor(cellY);
			break;
		}
		case 2: {
			x = floor(cellX);
			y = ceil(cellY);
			break;
		}
		case 3: {
			x = ceil(cellX);
			y = ceil(cellY);
		}
	}
}


unsigned char Collisions::map_collision(float x, float y, const Map& aMap, bool isBig) {
	float cellX = x / CELL_SIZE;
	float cellY = y / CELL_SIZE;
	unsigned char output = 0;
	for (unsigned char i = 0; i < 4; i++) {
		get_x_y(x, y, i, cellX, cellY, isBig);
		if (x >= 0 && x < aMap.size()){
			if (0 <= y && y < aMap[0].size()) {
				if (!isBig) {
					if (colisionCell.end() != std::find(colisionCell.begin(), colisionCell.end(), aMap[x][y])) output += pow(2, i);
				}
				else {
					if (colisionCell.end() != std::find(colisionCell.begin(), colisionCell.end(), aMap[x][y])) output += pow(2, i);
					else {
						if (y > 0) {
							if (colisionCell.end() != std::find(colisionCell.begin(), colisionCell.end(), aMap[x][y + 1])) output += pow(2, i);
						}
					}
				}
			}
		}
		else if (colisionCell.end() != std::find(colisionCell.begin(), colisionCell.end(), Cell::Wall)) output += pow(2, i);
	}
	return output;
}


void Collisions::coin_collision(std::vector<sf::Vector2i>& aCollisionCells, float x, float y, Map& aMap, bool isBig, unsigned int& count) {
	{
		float cellX = x / CELL_SIZE;
		float cellY = y / CELL_SIZE;
		aCollisionCells.clear();

		for (unsigned char i = 0; i < 4; i++)
		{
			short x;
			short y;
			switch (i) {
			case 0: {
				x = floor(cellX);
				y = floor(cellY);
				break;
			}
			case 1: {
				x = ceil(cellX);
				y = floor(cellY);
				break;
			}
			case 2: {
				x = floor(cellX);
				y = ceil(cellY);
				if (isBig)	y = ceil(cellY + 1);
				break;
			}
			case 3: {
				x = ceil(cellX);
				y = ceil(cellY);
				if (isBig)	y = ceil(cellY + 1);
			}
			}
			if (x >= 0 && x < aMap.size()) {
				if (0 <= y && y < aMap[0].size()) {
					if (aMap[x][y] == Cell::Coin) {
						aCollisionCells.push_back(sf::Vector2i(x, y));
						count += 100;
					}
				}
			}
		}
	}
}



void Collisions::get_collision_brick(std::vector<sf::Vector2i>& aCollisionCells, float x, float y, const Map& aMap) {
	float cellX = x / CELL_SIZE;
	float cellY = y / CELL_SIZE;
	aCollisionCells.clear();
	for (unsigned char i = 0; i < 4; i++){
		get_x_y(x, y, i, cellX, cellY, 1);
		if (x >= 0 && x < aMap.size()) {
			if (0 <= y && y < aMap[0].size()) {
				if (aMap[x][y] == Cell::Brick) {
					aCollisionCells.push_back(sf::Vector2i(x, y));
				}
			}
		}
	}
}


void Collisions::get_collision_question_block(std::vector<sf::Vector2i>& aCollisionCells, float x, float y, const Map& aMap, bool isBig) {
	float cellX = x / CELL_SIZE;
	float cellY = y / CELL_SIZE;
	aCollisionCells.clear();
	for (unsigned char i = 0; i < 4; i++) {
		get_x_y(x, y, i, cellX, cellY, isBig);
		if (x >= 0 && x < aMap.size()) {
			if (0 <= y && y < aMap[0].size()) {
				if (aMap[x][y] == Cell::QuestionBlock) {
					aCollisionCells.push_back(sf::Vector2i(x, y));
				}
			}
		}
	}
}