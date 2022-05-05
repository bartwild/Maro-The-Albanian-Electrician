#include "Animation.h"


Animation::Animation(unsigned short aSpeed, unsigned short aWidth, const std::string& aTextureLocation){
	speed = aSpeed; width = aWidth; texture.loadFromFile(aTextureLocation);
	iterator = 0; flipped = 0; frame = 0; totalFrames = texture.getSize().x / width;
}


void Animation::draw(short x, short y, sf::RenderWindow& window) {
	sprite.setPosition(x, y);
	sprite.setTexture(texture);
	if (flipped) {
		sprite.setTextureRect(sf::IntRect(width * (1 + frame), 0, -width, texture.getSize().y));
	}
	else {
		sprite.setTextureRect(sf::IntRect(width * (1 + frame), 0, width, texture.getSize().y));
	}
	window.draw(sprite);
}


void Animation::set_flipped(bool value){
	flipped = value;
}

void Animation::step() {
	iterator++;
	while (iterator >= speed) {
		iterator -= speed;
		frame = (1 + frame) % totalFrames;
	}
}