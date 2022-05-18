#include "Roomba.h"


Roomba::Roomba(){
	x = 100;
	y = 100;
	ySpeed = 0;
	xSpeed = ROOMBA_SPEED;
	walkAnimation = Animation(CELL_SIZE, "GoombaWalk.png", QUESTION_BLOCK_ANIMATION_SPEED);
	dead = 0;
	texture = std::make_shared<sf::Texture>();
	deathTimer = MARO_DEATH_TIMER/2;
	walkingOnRoomba = 0;
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


void Roomba::update(const Map& aMap, const unsigned aViewX, std::vector<std::shared_ptr<Roomba>> aRoombas){
    if ((-CELL_SIZE < y && x >= static_cast<int>(aViewX) - CELL_SIZE - UPDATE_AREA && x < UPDATE_AREA + SCREEN_WIDTH + aViewX && y < SCREEN_HEIGHT)
		&& (dead == 0)){
        bool moving = 0;
        unsigned char xCollision;
        unsigned char yCollision;
        xCollision = Collisions::map_collision(xSpeed + x, y, aMap, 0);
        if (xCollision != 0){
            if (5 & ~xCollision && 10 & xCollision){
                x = CELL_SIZE * (ceil((xSpeed + x) / CELL_SIZE) - 1);
            }
            else if (5 & xCollision && 10 & ~xCollision){
                x = CELL_SIZE * (1 + floor((xSpeed + x) / CELL_SIZE));
            }
            xSpeed *= -1;
        }
		sf::FloatRect HitBox(x + xSpeed, y + ySpeed, CELL_SIZE, CELL_SIZE);
		bool hit = 0;
		for (std::shared_ptr<Roomba> roomba : aRoombas){ //const nie dziala???
			if (roomba != shared_from_this() && HitBox.intersects(roomba->get_hit_box()) == 1){
				if (ySpeed > 0 || walkingOnRoomba == 1){
					y = -roomba->get_hit_box().height + roomba->get_hit_box().top;
					ySpeed = 0;
					walkingOnRoomba = 1;
				}
				else if (roomba->get_walkingOnRoomba() == 0){
					hit = 1;
					xSpeed *= -1;
				}
				break;
			}
			else{
				walkingOnRoomba = 0;
			}
		}
		if (hit == 0){
        	x += xSpeed;
		}
		if (walkingOnRoomba == 0){
			yCollision = Collisions::map_collision(x, 1 + y, aMap, 0);
			if (yCollision == 0){
				ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
			}
			yCollision = Collisions::map_collision(x, ySpeed + y, aMap, 0);
			if (yCollision > 0){
				if (3 & yCollision && 12 & ~yCollision){
					y = CELL_SIZE * (1 + floor((ySpeed + y) / CELL_SIZE));
				}
				else if (3 & ~yCollision && 12 & yCollision){
					y = CELL_SIZE * (ceil((ySpeed + y) / CELL_SIZE) - 1);
				}
				ySpeed = 0;
			}
		}
		y += ySpeed;
		if (y > SCREEN_WIDTH - 2*CELL_SIZE){
			die(1);
		}
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

void Roomba::die(bool instant){
	if (instant == 1){
		deathTimer = 0;
	}
	else{
		dead = 1;
		texture->loadFromFile("GoombaDeath.png");
		sprite.setTexture(*texture);
	}
}

unsigned char Roomba::get_death_timer() const{
	return deathTimer;
}

bool Roomba::get_walkingOnRoomba() const{
	return walkingOnRoomba;
}