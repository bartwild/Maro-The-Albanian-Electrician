#include "Maro.h"
#include "Consts.h"


Maro::Maro() {
	x = (0.5f * SCREEN_WIDTH);
	y = (0.5f * SCREEN_HEIGHT);
	texture.loadFromFile("BigMarioIdle.png");
	sprite.setTexture(texture);
}


void Maro::draw(sf::RenderWindow& aWindow) {
	sprite.setPosition(round(x), round(y));
	aWindow.draw(sprite);
}


void Maro::update() {
	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		x -= MARO_SPEED;
	}
	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		x += MARO_SPEED;
	}
}


float Maro::get_x() const { return x; }