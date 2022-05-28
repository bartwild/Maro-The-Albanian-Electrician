#pragma once
#include "Maro.h"
#include "Roomba.h"
#include "Consts.h"
#include "LevelManager.h"


class GameManager{
    std::vector<std::shared_ptr<Roomba>> roombas;
    std::vector<std::shared_ptr<Mushroom>> mushrooms;
    std::shared_ptr<Maro> maro;
    Map map;
    std::shared_ptr<LevelManager> levelManager;
public:
    GameManager(std::shared_ptr<Maro> aMaro, Map& aMap, std::shared_ptr<LevelManager> aLevelManager, std::vector<std::shared_ptr<Roomba>>& aRoombas, std::vector<std::shared_ptr<Mushroom>>& aMushrooms);
    void update_objects(unsigned int& aViewX, unsigned int& aCount);
    void draw(sf::RenderWindow& aWindow, unsigned int& aViewX, sf::View& aView, sf::Color& aBackgroundColor, sf::Texture& aMapTexture);
    void change_level(unsigned short& aLevelFinish, unsigned short& aCurrentLevel, sf::Image& aMapSketch, sf::Color& aBackgroundColor, unsigned int& aCount, unsigned int& aLastLevelPoints, sf::Time elapsed1, float& aLastLevelTime);
    std::vector<std::shared_ptr<Mushroom>> get_mushrooms();
    std::vector<std::shared_ptr<Roomba>> get_roombas();
    std::shared_ptr<Maro> get_maro();
    std::shared_ptr<LevelManager> get_level_manager();
    Map get_map();
};