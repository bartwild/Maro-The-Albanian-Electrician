#include "Maro.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <chrono>
#include <cmath>
#include "Consts.h"


int main() {
	Maro maro;
	sf::Color background_color = sf::Color(0, 210, 255);
	sf::Event event;
	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE*SCREEN_WIDTH, SCREEN_RESIZE*SCREEN_HEIGHT), "Maro The Albanian Electrician", sf::Style::Close);
	while (window.isOpen()) {
		window.clear(background_color);
		maro.draw(window);
		window.display();
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
	return 0;
}