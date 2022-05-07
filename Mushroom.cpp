#include "Mushroom.h"

unsigned char map_collision_mushroom(float x, float y, const Map& aMap) { //5-lewo. 10 prawo, 3 gora, 12 dol
	float cellX = x / CELL_SIZE;
	float cellY = y / CELL_SIZE;
	unsigned char output = 0;

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
			break;
		}
		case 3: {
			x = ceil(cellX);
			y = ceil(cellY);
		}
		}
		if (x >= 0 && x < aMap.size()) {
			if (0 <= y && y < aMap[0].size()) {
				if (colisionCell.end() != std::find(colisionCell.begin(), colisionCell.end(), aMap[x][y])) output += pow(2, i);
			}
		}
		else if (colisionCell.end() != std::find(colisionCell.begin(), colisionCell.end(), Cell::Wall)) output += pow(2, i);
	}
	return output;
}


Mushroom::Mushroom(const float aX, const float aY) {
	dead = 0;
	x = aX;
	y = aY;
	direction = 1;
	xSpeed = 0;
	startY = y;
	ySpeed = 0;
	texture.loadFromFile("Mushroom.png");
}


bool Mushroom::get_dead() const { return dead; }


sf::FloatRect Mushroom::get_hit_box() const { return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE); }


void Mushroom::draw(const unsigned aViewX, sf::RenderWindow& aWindow) {
	if (-CELL_SIZE < round(y) && round(x) > static_cast<int>(aViewX) - CELL_SIZE && round(x) < SCREEN_WIDTH + aViewX && round(y) < SCREEN_HEIGHT) {
		sprite.setPosition(round(x), round(y));
		sprite.setTexture(texture);
		aWindow.draw(sprite);
	}
}


void Mushroom::die(const bool isDead) { dead = isDead; }


void Mushroom::move(const unsigned aViewX, const Map& aMap) {
	if (!dead) {
		if (-CELL_SIZE < y && x >= static_cast<int>(aViewX) - CELL_SIZE - UPDATE_AREA && x < UPDATE_AREA + SCREEN_WIDTH + aViewX && y < SCREEN_HEIGHT) {
			if (y > startY - CELL_SIZE)
			{
				y -= MUSHROOM_SPEED;

				if (y <= startY - CELL_SIZE)
				{
					y = startY - CELL_SIZE;

					startY = 2 * SCREEN_HEIGHT;
				}
			}
			else {
				unsigned char xCollision;
				unsigned char yCollision;
				ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
				xCollision = map_collision_mushroom(xSpeed + x, y, aMap);
				if (xCollision != 0) {
					if (5 & ~xCollision && 10 & xCollision)
					{
						x = CELL_SIZE * (ceil((xSpeed + x) / CELL_SIZE) - 1);
					}
					else if (5 & xCollision && 10 & ~xCollision)
					{
						x = CELL_SIZE * (1 + floor((xSpeed + x) / CELL_SIZE));
					}
					xSpeed *= -1;
				}
				else {
					x += xSpeed;
				}
				yCollision = map_collision_mushroom(x, 1 + y, aMap);
				if (yCollision == 0) {
					ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
				}
				yCollision = map_collision_mushroom(x, ySpeed + y, aMap);
				if (yCollision > 0) {
					if (3 & yCollision && 12 & ~yCollision) {
						y = CELL_SIZE * (1 + floor((ySpeed + y) / CELL_SIZE));
					}
					else if (3 & ~yCollision && 12 & yCollision) {
						y = CELL_SIZE * (ceil((ySpeed + y) / CELL_SIZE) - 1);
					}
					ySpeed = 0;
				}
				else {
					y += ySpeed;
				}
			}
		}
		if (SCREEN_HEIGHT <= y)
		{
			dead = 1;
		}
	}
}
