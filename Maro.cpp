#include "Maro.h"



unsigned char map_collision(float x, float y, const Map& aMap) { //5-lewo. 10 prawo, 3 gora, 12 dol
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

Maro::Maro() {
	onGround = 0,
	x = 100;
	y = 100;
	ySpeed = 0;
	xSpeed = 0;
	jumpTimer = 0;
	texture.loadFromFile("MarioIdle.png");
	sprite.setTexture(texture);
}


void Maro::draw(sf::RenderWindow& aWindow) {
	sprite.setPosition(round(x), round(y));
	aWindow.draw(sprite);
}


void Maro::move(const Map& aMap) {
	bool moving = 0;
	bool onGround = 0;
	unsigned char xCollision;
	unsigned char yCollision;
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		moving = 1;
		xSpeed = std::min(xSpeed + MARO_ACCELERATION, MARO_SPEED);
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		moving = 1;
		xSpeed = std::max(xSpeed - MARO_ACCELERATION, -MARO_SPEED);
	}
	if (moving == 0){
		if (0 < xSpeed) {
			xSpeed = std::max<float>(0, xSpeed - MARO_ACCELERATION);
		}
		else if (0 > xSpeed) {
			xSpeed = std::min<float>(0, MARO_ACCELERATION + xSpeed);
		}
	}
	xCollision = map_collision(xSpeed + x, y, aMap);
	if (xCollision != 0) {
		moving = 0;
		if (5 & ~xCollision && 10 & xCollision)
		{
			x = CELL_SIZE * (ceil((xSpeed + x) / CELL_SIZE) - 1);
		}
		else if (5 & xCollision && 10 & ~xCollision)
		{
			x = CELL_SIZE * (1 + floor((xSpeed + x) / CELL_SIZE));
		}
		xSpeed = 0;
	}
	else {
		x += xSpeed;
	}
	yCollision = map_collision(x, 1 + y, aMap);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		if (ySpeed == 0 && yCollision > 0){
			ySpeed = MARO_JUMP_SPEED;
			jumpTimer = MARO_JUMP_TIMER;
		}
		else if (jumpTimer > 0){
			ySpeed = MARO_JUMP_SPEED;
			jumpTimer--;
		}
		else{
			ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
		}
	}
	else{
		ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
		jumpTimer = 0;
	}
	yCollision = map_collision(x, ySpeed + y, aMap);

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
	yCollision = map_collision(x, 1 + y, aMap);
	if (yCollision > 0) {
		onGround = 1;
	}
}


float Maro::get_x() const { return x; }


float Maro::get_y() const { return y; }


void Maro::set_position(float x, float y) {
	x = x;
	y = y;
}