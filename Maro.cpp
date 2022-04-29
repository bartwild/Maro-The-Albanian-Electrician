#include "Maro.h"


unsigned char map_collision(float x, float y, const Map& aMap) { //5-lewo. 10 prawo, 3 gora, 12 dol
	float cell_x = x / CELL_SIZE;
	float cell_y = y / CELL_SIZE;
	unsigned char output = 0;

	for (unsigned char i = 0; i < 4; i++)
	{
		short x;
		short y;
		switch (i) {
			case 0: {
				x = floor(cell_x);
				y = floor(cell_y);
				break;
			}
			case 1: {
				x = ceil(cell_x);
				y = floor(cell_y);
				break;
			}
			case 2: {
				x = floor(cell_x);
				y = ceil(cell_y);
				break;
			}
			case 3: {
				x = ceil(cell_x);
				y = ceil(cell_y);
			}
		}

		if (0 <= x && x < aMap.size()){
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
	texture.loadFromFile("BigMarioIdle.png");
	sprite.setTexture(texture);
}


void Maro::draw(sf::RenderWindow& aWindow) {
	sprite.setPosition(round(x), round(y));
	aWindow.draw(sprite);
}


void Maro::move(const Map& aMap) {
	bool moving = 0;
	bool onGround = 0;
	unsigned char horizontal_collision;
	unsigned char vertical_collision;
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		moving = 1;
		xSpeed = std::max(xSpeed - MARO_ACCELERATION, -MARO_SPEED);
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		moving = 1;
		xSpeed = std::min(xSpeed + MARO_ACCELERATION, MARO_SPEED);
	}
	if (0 == moving)
	{
		if (0 < xSpeed) {
			xSpeed = std::max<float>(0, xSpeed - MARO_ACCELERATION);
		}
		else if (0 > xSpeed) {
			xSpeed = std::min<float>(0, MARO_ACCELERATION + xSpeed);
		}
	}
	horizontal_collision = map_collision(xSpeed + x, y, aMap);
	if (horizontal_collision != 0) {
		moving = 0;
		if (5 & ~horizontal_collision && 10 & horizontal_collision)
		{
			x = CELL_SIZE * (ceil((xSpeed + x) / CELL_SIZE) - 1);
		}
		else if (5 & horizontal_collision && 10 & ~horizontal_collision)
		{
			x = CELL_SIZE * (1 + floor((xSpeed + x) / CELL_SIZE));
		}
		xSpeed = 0;
	}
	else {
		x -= xSpeed;
	}
	vertical_collision = map_collision(x, 1 + y, aMap);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		if (0 == ySpeed && 0 < vertical_collision){
			ySpeed = MARO_JUMP_SPEED;
			jumpTimer = MARO_JUMP_TIMER;
		}
		else if (0 < jumpTimer){
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
	vertical_collision = map_collision(x, ySpeed + y, aMap);

	if (0 < vertical_collision)
	{
		if (3 & vertical_collision && 12 & ~vertical_collision)
		{
			y = CELL_SIZE * (1 + floor((ySpeed + y) / CELL_SIZE));
		}
		else if (3 & ~vertical_collision && 12 & vertical_collision)
		{
			y = CELL_SIZE * (ceil((ySpeed + y) / CELL_SIZE) - 1);
		}

		ySpeed = 0;
	}
	else
	{
		y += ySpeed;
	}
	vertical_collision = map_collision(x, 1 + y, aMap);
	if (0 < vertical_collision) {
		onGround = 1;
	}
}


float Maro::get_x() const { return x; }
float Maro::get_y() const { return y; }