#include "Maro.h"
#include "Roomba.h"
#include "Consts.h"
#include "LevelManager.h"

void whole_Game() {
	Maro maro;
	std::vector<std::shared_ptr<Roomba>> roombas;
	unsigned viewX;
	sf::Event event;
	sf::Clock clock;
	sf::Time elapsed1;
	sf::Image mapSketch;
	sf::Texture mapTexture;
	unsigned int timeInt;
	unsigned int count = 0;
	unsigned short timer = 0;
	unsigned char levelFinish = 0;
	unsigned char currentLevel = 1;
	sf::Color backgroundColor = sf::Color(0, 219, 255);
	mapSketch.loadFromFile("LevelSketch0.png");
	LevelManager levelManager(mapSketch);
	std::chrono::microseconds lag(0);
	std::chrono::steady_clock::time_point previousTime;
	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Maro The Albanian Electrician", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));
	window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));
	previousTime = std::chrono::steady_clock::now();;
	Map map = levelManager.sketch_to_map(maro, levelFinish, backgroundColor, roombas);
	mapTexture.loadFromFile("Map.png");
	while (window.isOpen()){
		std::chrono::microseconds deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previousTime);
		lag += deltaTime;
		previousTime += deltaTime;
		while (FRAME_DURATION <= lag) {
			lag -= FRAME_DURATION;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed){
					window.close();
				}
			}
			if ((CELL_SIZE * levelFinish <= maro.get_x() && currentLevel == 0) || (currentLevel == 1 && CELL_SIZE * levelFinish <= maro.get_x() && maro.get_y() >= SCREEN_HEIGHT - 6 * CELL_SIZE)) {
				currentLevel++;
				roombas.clear();
				maro.reset();
				mapSketch.loadFromFile("LevelSketch" + std::to_string(currentLevel) + ".png");
				levelManager.set_sketch(mapSketch);
				map = levelManager.sketch_to_map(maro, levelFinish, backgroundColor, roombas);
			}
			viewX = std::clamp<int>(round(maro.get_x()) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * map.size() - SCREEN_WIDTH);
			maro.update(levelManager, viewX, map, roombas, count);
			for (unsigned short i = 0; i < roombas.size(); i++) {
				if (roombas[i]->get_death_timer() == 0) {
					roombas.erase(roombas.begin() + i);
				}
			}
			for (std::shared_ptr<Roomba> roomba : roombas) {
				roomba->update(map, viewX, roombas);
			}
			if (FRAME_DURATION > lag) {
				view.reset(sf::FloatRect(viewX, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
				window.setView(view);
				window.clear(backgroundColor);
				levelManager.draw_map(1, viewX, window, backgroundColor, mapTexture, map);
				maro.draw_mushrooms(viewX, window);
				levelManager.draw_map(0, viewX, window, backgroundColor, mapTexture, map);
				levelManager.update();
				maro.draw(window);
				elapsed1 = clock.getElapsedTime();
				if (currentLevel !=2) timeInt = int(elapsed1.asSeconds());
				for (std::shared_ptr<Roomba> roomba : roombas){
					roomba->draw(window);
				}
				if (currentLevel == 2) {
					std::string message = "\t    You won.\n   Congratulations.\n press Enter to reset.";
					sf::Font font;
					if (!font.loadFromFile("arial.ttf")) {}
					sf::Text text(message, font, 20);
					text.setPosition(sf::Vector2f(viewX + (SCREEN_WIDTH / SCREEN_RESIZE) - 10, 0));
					text.setFillColor(sf::Color(0, 0, 0));
					window.draw(text);
					count += (360-timeInt) * 50;
					timeInt = 360;
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
						window.close();
						whole_Game();
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || timer != 0) {
					timer = 1;
					std::string message = "\t   Are you sure? \n    press enter if yes\npress anything else if no";
					sf::Font font;
					if (!font.loadFromFile("arial.ttf")) {}
					sf::Text text(message, font, 20);
					text.setPosition(sf::Vector2f(viewX + (SCREEN_WIDTH / SCREEN_RESIZE) - 10, 0));
					text.setFillColor(sf::Color(0, 0, 0));
					window.draw(text);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
						window.close();
						whole_Game();
					}
					if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						for( auto& element : keysNoEscAndEnt)
							if (sf::Keyboard::isKeyPressed(element)) timer = 0;
				}
				if (!maro.get_death_timer() || (int(360 - elapsed1.asSeconds()) == 0)) {
					std::string message = "You lost, press\n enter to reset.";
					sf::Font font;
					if (!font.loadFromFile("arial.ttf")) {}
					sf::Text text(message, font, 30);
					text.setPosition(sf::Vector2f(viewX + (SCREEN_WIDTH / SCREEN_RESIZE) - 30, 0));
					text.setFillColor(sf::Color(0, 0, 0));
					window.draw(text);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
						window.close();
						whole_Game();
					}
				}
				if (currentLevel == 2 && (int(360 - elapsed1.asSeconds()) == 0)) {
					window.close();
					whole_Game();
				}
				std::string time = "Time left:  " + std::to_string(int(360 - elapsed1.asSeconds()));
				std::string message = "Points: " + std::to_string(count);
				sf::Font font;
				if (!font.loadFromFile("arial.ttf")) {}
				sf::Text text(message, font, 12);
				sf::Text timeText(time, font, 12);
				timeText.setPosition(sf::Vector2f(viewX + (3 * SCREEN_WIDTH / SCREEN_RESIZE), 12));
				text.setPosition(sf::Vector2f(viewX + (3 * SCREEN_WIDTH / SCREEN_RESIZE), 0));
				text.setFillColor(sf::Color(0, 0, 0));
				timeText.setFillColor(sf::Color(0, 0, 0));
				window.draw(text);
				window.draw(timeText);
				window.display();
			}
		}
	}
}


int main() {
	whole_Game();
	return 0;
}