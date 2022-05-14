#include "Maro.h"
#include "Roomba.h"
#include "Consts.h"
#include "LevelManager.h"

void whole_Game() {
	Maro maro;
	std::vector<Roomba> roombas;
	unsigned viewX;
	sf::Event event;
	sf::Image mapSketch;
	sf::Texture mapTexture;
	sf::Texture questionBlock;
	unsigned char levelFinish = 0;
	unsigned char currentLevel = 0;
	sf::Color backgroundColor = sf::Color(0, 219, 255);
	mapSketch.loadFromFile("LevelSketch0.png");
	LevelManager levelManager(mapSketch);
	std::chrono::microseconds lag(0);
	std::chrono::steady_clock::time_point previousTime;
	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Maro The Albanian Electrician", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));
	window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));
	questionBlock.loadFromFile("QuestionBlock.png");
	previousTime = std::chrono::steady_clock::now();;
	Map map = levelManager.sketch_to_map(maro, levelFinish, backgroundColor, roombas);
	mapTexture.loadFromFile("Map.png");
	while (window.isOpen()) {
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
			if (CELL_SIZE * levelFinish <= maro.get_x()) {
				currentLevel++;
				roombas.clear();
				maro.reset();
				mapSketch.loadFromFile("LevelSketch" + std::to_string(currentLevel) + ".png");
				levelManager.set_sketch(mapSketch);
				map = levelManager.sketch_to_map(maro, levelFinish, backgroundColor, roombas);
			}
			viewX = std::clamp<int>(round(maro.get_x()) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * map.size() - SCREEN_WIDTH);
			maro.update(levelManager, viewX, map, roombas);
			for (Roomba& roomba : roombas){
				roomba.move(map, viewX, roombas);
			}
			for (unsigned short i = 0; i < roombas.size(); i++){
				if (roombas[i].get_death_timer() == 0){
					roombas.erase(i+roombas.begin());
					i--;
				}
			}
			if (FRAME_DURATION > lag) {
				view.reset(sf::FloatRect(viewX, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
				window.setView(view);
				window.clear(backgroundColor);
				levelManager.draw_map(1, viewX, window, backgroundColor, mapTexture, questionBlock, map);
				maro.draw_mushrooms(viewX, window);
				levelManager.draw_map(0, viewX, window, backgroundColor, mapTexture, questionBlock, map);
				levelManager.update();
				maro.draw(window);
				for (Roomba& roomba : roombas){
					roomba.draw(window);
				}
				if (!maro.get_death_timer()) {
					std::string message = "You lost, press\n enter to reset.";
					sf::Font font;
					if (!font.loadFromFile("arial.ttf")) {}
					sf::Text text(message, font, 30);
					text.setPosition(sf::Vector2f(maro.get_x()-((SCREEN_WIDTH)/SCREEN_RESIZE), 0));
					text.setFillColor(sf::Color(0, 0, 0));
					window.draw(text);
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
						window.close();
						whole_Game();
					}
				}
				window.display();
			}
		}
	}
}


int main() {
	whole_Game();
	return 0;
}