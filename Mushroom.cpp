#include "Mushroom.h"


Mushroom::Mushroom(const float aX, const float aY) {
	dead = 0;
	x = aX;
	y = aY;
	xSpeed = MUSHROOM_SPEED;
	startY = y;
	ySpeed = 0;
	texture.loadFromFile("Mushroom.png");
}


void Mushroom::draw(const unsigned aViewX, sf::RenderWindow& aWindow) {
	if (-CELL_SIZE < round(y) && round(x) > static_cast<int>(aViewX) - CELL_SIZE && round(x) < SCREEN_WIDTH + aViewX && round(y) < SCREEN_HEIGHT) {
		sprite.setPosition(round(x), round(y));
		sprite.setTexture(texture);
		aWindow.draw(sprite);
	}
}


void Mushroom::die(const bool isDead) {
	dead = isDead;
}


void Mushroom::move(const unsigned aViewX, const Map& aMap) {
	if (!dead) {
		if (-CELL_SIZE < y && x >= static_cast<int>(aViewX) - CELL_SIZE - UPDATE_AREA && x < UPDATE_AREA + SCREEN_WIDTH + aViewX && y < SCREEN_HEIGHT) {
			if (y > startY - CELL_SIZE) {
				y -= MUSHROOM_SPEED;
				if (y <= startY - CELL_SIZE) {
					y = startY - CELL_SIZE;
					startY = 2 * SCREEN_HEIGHT;
				}
			}
			else {
				unsigned char xCollision;
				unsigned char yCollision;
				ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
				xCollision = Collisions::map_collision(xSpeed + x, y, aMap, 0);
				if (xCollision != 0) {
					if (5 & ~xCollision && 10 & xCollision)	{
						x = CELL_SIZE * (ceil((xSpeed + x) / CELL_SIZE) - 1);
					}
					else if (5 & xCollision && 10 & ~xCollision) {
						x = CELL_SIZE * (1 + floor((xSpeed + x) / CELL_SIZE));
					}
					xSpeed *= -1;
				}
				else {
					x += xSpeed;
				}
				yCollision = Collisions::map_collision(x, 1 + y, aMap, 0);
				if (yCollision == 0) {
					ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
				}
				yCollision = Collisions::map_collision(x, ySpeed + y, aMap, 0);
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
		if (y >= SCREEN_HEIGHT) {
			dead = 1;
		}
	}
}


bool Mushroom::get_whether_dead() const {
	return dead;
}


sf::FloatRect Mushroom::get_hit_box() const {
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}


float Mushroom::get_x() const {
	return x;
}


float Mushroom::get_y() const {
	return y;
}


float Mushroom::get_xSpeed() const {
	return xSpeed;
}


float Mushroom::get_ySpeed() const {
	return ySpeed;
}


void Mushroom::set_x_y(float newX, float newY) {
	x = newX;
	y = newY;
}