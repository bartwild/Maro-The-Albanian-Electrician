#include "Maro.h"
#include "Consts.h"


Maro::Maro() {
	x = (0.5f * SCREEN_RESIZE*SCREEN_WIDTH);
	y = (0.5f * SCREEN_RESIZE*SCREEN_HEIGHT);
	texture.loadFromFile("BigMarioIdle.png");
	sprite.setTexture(texture);
}


void Maro::draw(sf::RenderWindow& i_window) {
	sprite.setPosition(round(x), round(y));
	i_window.draw(sprite);
}
