#include "GameManager.h"

GameManager::GameManager(std::shared_ptr<Maro> aMaro, Map& aMap, std::shared_ptr<LevelManager> aLevelManager, std::vector<std::shared_ptr<Roomba>>& aRoombas, std::vector<std::shared_ptr<Mushroom>>& aMushrooms) {
    maro = aMaro;
    levelManager = aLevelManager;
    roombas = aRoombas;
    mushrooms = aMushrooms;
    map = aMap;
}


void GameManager::update_objects(unsigned int& aViewX, unsigned int& aCount) {
	maro->update(*levelManager, aViewX, map, roombas, mushrooms, aCount);
	for (unsigned short i = 0; i < roombas.size(); i++) {
		if (roombas[i]->get_death_timer() == 0) {
			roombas.erase(roombas.begin() + i);
		}
	}
	for (std::shared_ptr<Roomba> roomba : roombas) {
		roomba->update(map, aViewX, roombas);
	}
	for (std::shared_ptr<Mushroom> mushroom : mushrooms) {
		mushroom->move(aViewX, map);
	}
}


void GameManager::draw(sf::RenderWindow& aWindow, unsigned int& aViewX, sf::View& aView, sf::Color& aBackgroundColor, sf::Texture& aMapTexture) {
    aView.reset(sf::FloatRect(aViewX, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	aWindow.setView(aView);
	aWindow.clear(aBackgroundColor);
	levelManager->draw_map(1, aViewX, aWindow, aBackgroundColor, aMapTexture, map);
	for (std::shared_ptr<Mushroom> mushroom : mushrooms) {
		mushroom->draw(aViewX, aWindow);
	}
	levelManager->draw_map(0, aViewX, aWindow, aBackgroundColor, aMapTexture, map);
	levelManager->update();
	maro->draw(aWindow);
	for (std::shared_ptr<Roomba> roomba : roombas) {
		roomba->draw(aWindow);
	}
}



void GameManager::change_level(unsigned short& aLevelFinish, unsigned short& aCurrentLevel, sf::Image& aMapSketch, sf::Color& aBackgroundColor) {
	if ((CELL_SIZE * aLevelFinish <= maro->get_x() && aCurrentLevel == 0) || (aCurrentLevel == 1 && CELL_SIZE * aLevelFinish <= maro->get_x() && maro->get_y() >= SCREEN_HEIGHT - 6 * CELL_SIZE)) {
		aCurrentLevel++;
		roombas.clear();
		maro->reset();
		aMapSketch.loadFromFile("LevelSketch" + std::to_string(aCurrentLevel) + ".png");
		levelManager->set_sketch(aMapSketch);
		map = levelManager->sketch_to_map(*maro, aLevelFinish, aBackgroundColor, roombas);
	}
}
