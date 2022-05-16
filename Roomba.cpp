#include "Roomba.h"


unsigned char map_collision_Roomba(float x, float y, const Map& aMap){ //5-lewo. 10 prawo, 3 gora, 12 dol
	float cellX = x / CELL_SIZE;
	float cellY = y / CELL_SIZE;
	unsigned char output = 0;
	for (unsigned char i = 0; i < 4; i++){
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

Roomba::Roomba(){
	x = 100;
	y = 100;
	ySpeed = 0;
	xSpeed = ROOMBA_SPEED;
	walkAnimation = Animation(CELL_SIZE, "GoombaWalk.png", QUESTION_BLOCK_ANIMATION_SPEED);
	dead = 0;
	texture = new sf::Texture;
	deathTimer = MARO_DEATH_TIMER/2;
}


void Roomba::draw(sf::RenderWindow& aWindow){
	if (!dead){
		walkAnimation.set_position(round(x), round(y));
		walkAnimation.step(1);
		walkAnimation.draw(aWindow);
	}
	else{
		sprite.setPosition(round(x), round(y));
		texture->loadFromFile("GoombaDeath.png");
		sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(texture->getSize().x), texture->getSize().y));
		aWindow.draw(sprite);
	}
}


void Roomba::update(const Map& aMap, const unsigned aViewX, std::vector<Roomba>& aRoombas){
    if ((-CELL_SIZE < y && x >= static_cast<int>(aViewX) - CELL_SIZE - UPDATE_AREA && x < UPDATE_AREA + SCREEN_WIDTH + aViewX && y < SCREEN_HEIGHT)
		&& (dead == 0)){
        bool moving = 0;
        unsigned char xCollision;
        unsigned char yCollision;
        xCollision = map_collision_Roomba(xSpeed + x, y, aMap);
        if (xCollision != 0){
            if (5 & ~xCollision && 10 & xCollision){
                x = CELL_SIZE * (ceil((xSpeed + x) / CELL_SIZE) - 1);
            }
            else if (5 & xCollision && 10 & ~xCollision){
                x = CELL_SIZE * (1 + floor((xSpeed + x) / CELL_SIZE));
            }
            xSpeed *= -1;
        }
		sf::FloatRect xHitBox(xSpeed + x, y, CELL_SIZE, CELL_SIZE);
		bool hit = 0;
		for (Roomba& roomba : aRoombas){ //const nie dziala???
			if (&roomba != this && xHitBox.intersects(roomba.get_hit_box()) == 1){
				hit = 1;
				xSpeed *= -1;
				break;
			}
		}
		if (hit == 0){
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
        y += ySpeed;
    }
	else if(dead == 1){
		deathTimer = std::max(0, deathTimer-1);
	}
}


float Roomba::get_x() const { return x; }


float Roomba::get_y() const { return y; }

float Roomba::get_ySpeed() const { return ySpeed; }

bool Roomba::get_whether_dead() const { return dead; }

void Roomba::set_position(float newX, float newY){
	x = newX;
	y = newY;
}

sf::FloatRect Roomba::get_hit_box() const{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}

void Roomba::die(){
	dead = 1;
	texture->loadFromFile("GoombaDeath.png");
	sprite.setTexture(*texture);
}

unsigned char Roomba::get_death_timer() const{
	return deathTimer;
}