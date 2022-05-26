#include "GameManager.h"

GameManager::GameManager(std::shared_ptr<Maro>& aMaro, Map& aMap, std::shared_ptr<LevelManager>& aLevelManager, std::vector<std::shared_ptr<Roomba>>& aRoombas, std::vector<std::shared_ptr<Mushroom>>& aMushrooms) {
    maro = aMaro;
    levelManager = aLevelManager;
    roombas = aRoombas;
    mushrooms = aMushrooms;
    map = aMap;
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