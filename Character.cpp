#include "Character.h"


float Character::get_x() const {
    return x;
}


float Character::get_y() const {
    return y;
}


float Character::get_xSpeed() const {
    return xSpeed;
}


float Character::get_ySpeed() const {
    return ySpeed;
}


void Character::set_position(float newX, float newY) {
    x = newX;
    y = newY;
}


sf::FloatRect Character::get_hit_box() const {
    return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}

