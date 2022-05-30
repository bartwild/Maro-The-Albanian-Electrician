#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Animation.h"
#include "Collisions.h"
#include "Consts.h"
#include "LevelManager.h"
#include "Maro.h"
#include "Mushroom.h"
#include "Roomba.h"
#include "GameManager.h"



TEST_CASE("Testing Maro") {


	SECTION("Testing Constructor") {
		Maro maro;
		sf::FloatRect hitBox = sf::FloatRect(100, 100, CELL_SIZE, CELL_SIZE);
		REQUIRE(maro.get_x() == 100);
		REQUIRE(maro.get_y() == 100);
		REQUIRE(maro.get_flipped() == 0);
		REQUIRE(maro.get_big() == 0);
		REQUIRE(maro.get_hit_box() == hitBox);
		REQUIRE(maro.get_xSpeed() == 0);
		REQUIRE(maro.get_ySpeed() == 0);
	}


	SECTION("Testing Setters") {
		Maro maro;
		maro.set_position(200, 200);
		REQUIRE(maro.get_x() == 200);
		REQUIRE(maro.get_y() == 200);
		REQUIRE(maro.get_xSpeed() == 0);
		REQUIRE(maro.get_ySpeed() == 0);
		maro.become_big();
		REQUIRE(maro.get_big() == 1);
		maro.become_small();
		REQUIRE(maro.get_big() == 0);
		sf::FloatRect hitBox = sf::FloatRect(200, 200, CELL_SIZE, CELL_SIZE);
		REQUIRE(maro.get_hit_box() == hitBox);
		maro.reset();
		hitBox = sf::FloatRect(0, 0, CELL_SIZE, CELL_SIZE);
		REQUIRE(maro.get_x() == 0);
		REQUIRE(maro.get_y() == 0);
		REQUIRE(maro.get_flipped() == 0);
		REQUIRE(maro.get_big() == 0);
		REQUIRE(maro.get_hit_box() == hitBox);
	}


	SECTION("Testing Mechanics") {
		std::shared_ptr<Mushroom> mushroom = std::make_shared<Mushroom>(32, 192);
		std::shared_ptr<Mushroom> mushroom2 = std::make_shared<Mushroom>(100, 100);
		std::vector<std::shared_ptr<Mushroom>> mushrooms;
		mushrooms.push_back(mushroom);
		mushrooms.push_back(mushroom2);
		unsigned int count = 0;
		unsigned short levelFinish = 0;
		sf::Color backgroundColor = sf::Color(0, 219, 255);
		sf::Image mapSketch;
		mapSketch.loadFromFile(MAP_PATH + "LevelSketch0.png");
		sf::Texture mapTexture;
		mapTexture.loadFromFile(MAP_PATH + "Map.png");
		std::vector<std::shared_ptr<Roomba>> roombas;
		std::shared_ptr<LevelManager> levelManager = std::make_shared<LevelManager>(mapSketch);
		std::shared_ptr<Maro> maroP = std::make_shared<Maro>();
		Map map = levelManager->sketch_to_map(*maroP, levelFinish, backgroundColor, roombas);
		std::vector<sf::Vector2i> collisionCells;
		GameManager gameManager(maroP, map, levelManager, roombas, mushrooms);
		unsigned viewX = std::clamp<int>(round(maroP->get_x()) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * map.size() - SCREEN_WIDTH);
		gameManager.update_objects(viewX, count);
		CHECK(count == 200);
		CHECK(maroP->get_big() == 1);
		CHECK(gameManager.get_mushrooms().size() == 1);
		maroP->set_position(100, 100);
		gameManager.update_objects(viewX, count);
		CHECK(count == 1200);
		CHECK(maroP->get_big() == 1);
		maroP->set_position(22 * CELL_SIZE, 11 * CELL_SIZE - 2);
		gameManager.update_objects(viewX, count);
		gameManager.update_objects(viewX, count);
		CHECK(gameManager.get_roombas()[0]->get_whether_dead() == 1);
		CHECK(gameManager.get_roombas().size() == 16);
		CHECK(count == 1400);
		unsigned int i = 0;
		while (i < 32) {
			gameManager.update_objects(viewX, count);
			i += 1;
		}
		CHECK(gameManager.get_roombas().size() == 15);
		CHECK(gameManager.get_roombas()[0]->get_whether_dead() == 0);
		maroP->set_position(40 * CELL_SIZE, 11 * CELL_SIZE);
		gameManager.update_objects(viewX, count);
		CHECK(gameManager.get_roombas()[0]->get_whether_dead() == 0);
		CHECK(maroP->get_big() == 0);
		while (i < 256) {
			gameManager.update_objects(viewX, count);
			i += 1;
		}
		CHECK(maroP->get_death_timer() == 0);
		CHECK(gameManager.get_roombas()[0]->get_whether_dead() == 0);
		maroP->reset();
		CHECK(maroP->get_death_timer() > 0);
		CHECK(maroP->get_on_ground() == 0);
		maroP->set_position(10 * CELL_SIZE, 12 * CELL_SIZE);
		gameManager.update_objects(viewX, count);
		CHECK(maroP->get_on_ground() == 1);
	}
}


TEST_CASE("Testing Roomba") {
	Maro maro;
	maro.set_position(600, 100);
	sf::Image mapSketch;
	sf::Texture mapTexture;
	mapTexture.loadFromFile(MAP_PATH + "Map.png");
	mapSketch.loadFromFile(MAP_PATH + "LevelSketch0.png");
	LevelManager levelManager(mapSketch);
	std::vector<std::shared_ptr<Roomba>> roombas;
	sf::Color backgroundColor = sf::Color(0, 219, 255);
	unsigned short levelFinish = 0;
	Map map = levelManager.sketch_to_map(maro, levelFinish, backgroundColor, roombas);
	unsigned viewX = 600;

    SECTION("testing constructor") {
        Roomba roomba;
        CHECK(roomba.get_walkingOnRoomba() == 0);
        CHECK(roomba.get_whether_dead() == 0);
        CHECK(roomba.get_ySpeed() == 0);
        CHECK(roomba.get_death_timer() == 32);
        CHECK(roomba.get_ySpeed() == 0);
        CHECK(roomba.get_xSpeed() == 1);
    }


    SECTION("testing setters") {
        Roomba roomba;
        roomba.set_position(10, 20);
        CHECK(roomba.get_x() == 10);
        CHECK(roomba.get_y() == 20);
		roomba.die(0);
		CHECK(roomba.get_whether_dead() == 1);
		roomba.die(1);
		CHECK(roomba.get_death_timer() == 0);
    }


	SECTION("testing update") {
		float xBeforeUpdate = roombas[1]->get_x();
		float yBeforeUpdate = roombas[1]->get_y();
		roombas[1]->update(map, viewX, roombas);
		CHECK(roombas[1]->get_x() == xBeforeUpdate + 1);
		CHECK(roombas[1]->get_y() == yBeforeUpdate);
	}


	SECTION ("testing Roomba interaction") {
		roombas[2]->set_position(40*CELL_SIZE, 11*CELL_SIZE-0.2);
		for (std::shared_ptr<Roomba> roomba : roombas) {
			roomba->update(map, viewX, roombas);
		}
		for (std::shared_ptr<Roomba> roomba : roombas) {
			roomba->update(map, viewX, roombas);
		}
		CHECK(roombas[2]->get_walkingOnRoomba() == 1);
	}
}


TEST_CASE("Testing mushroom") {


    SECTION("testing constructor") {
        Mushroom mushroom(0, 0);
        CHECK(mushroom.get_x() == 0);
        CHECK(mushroom.get_y() == 0);
        CHECK(mushroom.get_whether_dead() == 0);
        CHECK(mushroom.get_xSpeed() == 1);
        CHECK(mushroom.get_ySpeed() == 0);
    }


    SECTION("testing setters") {
        Mushroom mushroom(0, 0);
        mushroom.set_position(15, 20);
        CHECK(mushroom.get_x() == 15);
        CHECK(mushroom.get_y() == 20);
		mushroom.die(1);
		CHECK(mushroom.get_whether_dead() == 1);
    }


	SECTION("testing mushroom move") {
		std::shared_ptr<Maro> maro = std::make_shared<Maro>();;
		maro->set_position(600, 100);
		sf::Image mapSketch;
		sf::Texture mapTexture;
		mapTexture.loadFromFile(MAP_PATH + "Map.png");
		mapSketch.loadFromFile(MAP_PATH + "LevelSketch0.png");
		std::vector<std::shared_ptr<Mushroom>> mushrooms;
		std::shared_ptr<LevelManager> levelManager = std::make_shared<LevelManager>(mapSketch);
		std::vector<std::shared_ptr<Roomba>> roombas;
		sf::Color backgroundColor = sf::Color(0, 219, 255);
		unsigned short levelFinish = 0;
		Map map = levelManager->sketch_to_map(*maro, levelFinish, backgroundColor, roombas);
		GameManager gameManager(maro, map, levelManager, roombas, mushrooms);
		std::shared_ptr<Mushroom> mushroom = std::make_shared<Mushroom>(600 + 3*CELL_SIZE, 100);
		mushrooms.push_back(mushroom);
		unsigned viewX = 600;
		mushroom->move(viewX, map);
		CHECK(mushroom->get_x() == 600 + 3 * CELL_SIZE);
		CHECK(mushroom->get_y() == 99);
	}
}


TEST_CASE("Testing Level Manager") {
	 Maro maro;
        Roomba roomba1;
        Roomba roomba2;
        std::vector<Roomba> roombas = { roomba1, roomba2 };
        sf::Color backgroundColor = sf::Color(0, 219, 255);
        sf::Image mapSketch;
        mapSketch.loadFromFile(MAP_PATH + "LevelSketch0.png");
        LevelManager levelManager(mapSketch);


    SECTION("testing sketch to map level 0") {
        sf::Color pixel = levelManager.get_map_sketch_pixel(16, 9);
		CHECK((unsigned int)pixel.r == 255);
		CHECK((unsigned int)pixel.g == 146);
		CHECK((unsigned int)pixel.b == 85);
		pixel = levelManager.get_map_sketch_pixel(58, 10);
		CHECK((unsigned int)pixel.r == 0);
		CHECK((unsigned int)pixel.g == 182);
		CHECK((unsigned int)pixel.b == 0);
		pixel = levelManager.get_map_sketch_pixel(101, 9);
		CHECK((unsigned int)pixel.r == 182);
		CHECK((unsigned int)pixel.g == 73);
		CHECK((unsigned int)pixel.b == 0);
    }


	SECTION("testing sketch to map level 1") {
		mapSketch.loadFromFile(MAP_PATH + "LevelSketch1.png");
		levelManager.set_sketch(mapSketch);
        sf::Color pixel = levelManager.get_map_sketch_pixel(10, 9);
		CHECK((unsigned int)pixel.r == 255);
		CHECK((unsigned int)pixel.g == 73);
		CHECK((unsigned int)pixel.b == 85);
		pixel = levelManager.get_map_sketch_pixel(104, 12);
		CHECK((unsigned int)pixel.r == 0);
		CHECK((unsigned int)pixel.g == 182);
		CHECK((unsigned int)pixel.b == 0);
		pixel = levelManager.get_map_sketch_pixel(40, 8);
		CHECK((unsigned int)pixel.r == 255);
		CHECK((unsigned int)pixel.g == 255);
		CHECK((unsigned int)pixel.b == 0);
    }


	SECTION("testing sketch to map level 2") {
		mapSketch.loadFromFile(MAP_PATH + "LevelSketch2.png");
		levelManager.set_sketch(mapSketch);
        sf::Color pixel = levelManager.get_map_sketch_pixel(6, 8);
		CHECK((unsigned int)pixel.r == 146);
		CHECK((unsigned int)pixel.g == 73);
		CHECK((unsigned int)pixel.b == 0);
		pixel = levelManager.get_map_sketch_pixel(5, 8);
		CHECK((unsigned int)pixel.r == 255);
		CHECK((unsigned int)pixel.g == 146);
		CHECK((unsigned int)pixel.b == 85);
    }
}


TEST_CASE("Testing Collisions") {
	// Some stuff needeed for tests
	Maro maro;
	unsigned short levelFinish = 0;
	sf::Color backgroundColor = sf::Color(0, 219, 255);
	sf::Image mapSketch;
	sf::Image mapSketch2;
	mapSketch.loadFromFile(MAP_PATH + "LevelSketch0.png");
	mapSketch2.loadFromFile(MAP_PATH + "LevelSketch1.png");
	sf::Texture mapTexture;
	mapTexture.loadFromFile(MAP_PATH + "Map.png");
	std::vector<std::shared_ptr<Roomba>> roombas;
	LevelManager levelManager(mapSketch);
	LevelManager levelManager2(mapSketch2);
	Map map = levelManager.sketch_to_map(maro, levelFinish, backgroundColor, roombas);
	Map map2 = levelManager2.sketch_to_map(maro, levelFinish, backgroundColor, roombas);
	unsigned char right_collision = 10;
	std::vector<sf::Vector2i> collisionCells;
	unsigned int count;
	//


	SECTION("map collision - binary") {
		REQUIRE(Collisions::map_collision(100, 100, map, 0) == 0);
		REQUIRE(Collisions::map_collision(100, 14 * CELL_SIZE, map, 0) == 15);
		REQUIRE(Collisions::map_collision(22 * CELL_SIZE, 145, map, 0) == 3);
		REQUIRE(Collisions::map_collision(28 * CELL_SIZE - 1, 11 * CELL_SIZE, map, 0) == right_collision);
		REQUIRE(Collisions::map_collision(30 * CELL_SIZE - 1, 11 * CELL_SIZE, map, 0) == 5);
	}


	SECTION("coin collision") {
		REQUIRE(collisionCells.size() == 0);
		Collisions::coin_collision(collisionCells, 43 * CELL_SIZE, 5 * CELL_SIZE, map2, 0, count);
		REQUIRE(collisionCells[0].x == 43);
		REQUIRE(collisionCells[0].y == 5);
		REQUIRE(collisionCells[1].x == collisionCells[0].x);
		REQUIRE(collisionCells[1].y == collisionCells[0].y);
		REQUIRE(collisionCells[2].x == collisionCells[0].x);
		REQUIRE(collisionCells[2].y == collisionCells[0].y);
		REQUIRE(collisionCells[3].x == collisionCells[0].x);
		REQUIRE(collisionCells[3].y == collisionCells[0].y);
		REQUIRE(collisionCells.size() == 4);
	}


	SECTION("question block collision") {
		REQUIRE(collisionCells.size() == 0);
		Collisions::get_collision_question_block(collisionCells, 12 * CELL_SIZE, 9 * CELL_SIZE, map2, 0);
		REQUIRE(collisionCells[0].x == 12);
		REQUIRE(collisionCells[0].y == 9);
		REQUIRE(collisionCells[1].x == collisionCells[0].x);
		REQUIRE(collisionCells[1].y == collisionCells[0].y);
		REQUIRE(collisionCells[2].x == collisionCells[0].x);
		REQUIRE(collisionCells[2].y == collisionCells[0].y);
		REQUIRE(collisionCells[3].x == collisionCells[0].x);
		REQUIRE(collisionCells[3].y == collisionCells[0].y);
		REQUIRE(collisionCells.size() == 4);
	}


	SECTION("brick collision") {
		REQUIRE(collisionCells.size() == 0);
		Collisions::get_collision_brick(collisionCells, 29 * CELL_SIZE, 8 * CELL_SIZE, map2);
		REQUIRE(collisionCells[0].x == 29);
		REQUIRE(collisionCells[0].y == 8);
		REQUIRE(collisionCells[1].x == collisionCells[0].x);
		REQUIRE(collisionCells[1].y == collisionCells[0].y);
		REQUIRE(collisionCells[2].x == collisionCells[0].x);
		REQUIRE(collisionCells[2].y == collisionCells[0].y);
		REQUIRE(collisionCells[3].x == collisionCells[0].x);
		REQUIRE(collisionCells[3].y == collisionCells[0].y);
		REQUIRE(collisionCells.size() == 4);
	}

}