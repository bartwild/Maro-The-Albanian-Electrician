#include "Animation.h"


Animation::Animation(const  unsigned short aWidth, const std::string& aTextureLocation, const unsigned short aSpeed) {
	speed = aSpeed;
	width = aWidth;
	texture.loadFromFile(aTextureLocation);
	iterator = 0;
	flipped = 0;
	frame = 0;
	totalFrames = texture.getSize().x / width;
}


void Animation::draw(sf::RenderWindow& window) {
	sprite.setTexture(texture);
	if (flipped) {
		sprite.setTextureRect(sf::IntRect(width * (1 + frame), 0, -width, texture.getSize().y));
	}
	else {
		sprite.setTextureRect(sf::IntRect(width * (frame), 0, width, texture.getSize().y));
	}
	window.draw(sprite);
}


void Animation::set_flipped(bool value) {
	flipped = value;
}


void Animation::step(bool isBig) {
	if (isBig) {
		while (iterator >= speed) {
			iterator -= speed;
			frame = (1 + frame) % totalFrames;
		}
	}
	else {
		while (iterator >= speed) {
			iterator -= speed;
			frame = (2 + frame) % totalFrames;
		}
	}
	iterator++;
}


void Animation::set_position(float x, float y) {
	sprite.setPosition(x, y);
}