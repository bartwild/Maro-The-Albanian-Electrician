#include "Roomba.h"


unsigned char map_collision_Roomba(float x, float y, const Map& aMap) { //5-lewo. 10 prawo, 3 gora, 12 dol
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
		if (x >= 0 && x < aMap.size()){
			if (0 <= y && y < aMap[0].size()){
				if (colisionCell.end() != std::find(colisionCell.begin(), colisionCell.end(), aMap[x][y])) output += pow(2, i);
			}
		}
		else if (colisionCell.end() != std::find(colisionCell.begin(), colisionCell.end(), Cell::Wall)) output += pow(2, i);
	}
	return output;
}

Roomba::Roomba() {
	x = 100;
	y = 100;
	ySpeed = 0;
	xSpeed = ROOMBA_SPEED;
	texture.loadFromFile("GoombaDeath1.png");
	sprite.setTexture(texture);
}


void Roomba::draw(sf::RenderWindow& aWindow) {
	sprite.setPosition(round(x), round(y));
    sprite.setTexture(texture);
	aWindow.draw(sprite);
}


void Roomba::move(const Map& aMap, const unsigned aViewX){
    if (-CELL_SIZE < y && x >= static_cast<int>(aViewX) - CELL_SIZE - UPDATE_AREA && x < UPDATE_AREA + SCREEN_WIDTH + aViewX && y < SCREEN_HEIGHT){
        bool moving = 0;
        unsigned char xCollision;
        unsigned char yCollision;
        xCollision = map_collision_Roomba(xSpeed + x, y, aMap);
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
        yCollision = map_collision_Roomba(x, 1 + y, aMap);
        if (yCollision == 0){
            ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
        }
        yCollision = map_collision_Roomba(x, ySpeed + y, aMap);
        if (yCollision > 0){
            if (3 & yCollision && 12 & ~yCollision){
                y = CELL_SIZE * (1 + floor((ySpeed + y) / CELL_SIZE));
            }
            else if (3 & ~yCollision && 12 & yCollision){
                y = CELL_SIZE * (ceil((ySpeed + y) / CELL_SIZE) - 1);
            }
            ySpeed = 0;
        }
        else{
            y += ySpeed;
        }
    }
}


float Roomba::get_x() const { return x; }


float Roomba::get_y() const { return y; }


void Roomba::set_position(float newX, float newY) {
	x = newX;
	y = newY;
}