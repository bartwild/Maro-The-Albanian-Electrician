#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Animation.h"
#include "Collisions.h"
#include "Consts.h"
#include "LevelManager.h"
#include "Maro.h"
#include "Mushroom.h"
#include "Roomba.h"



TEST_CASE("Testing Maro") {


	SECTION("Testing Constructor") {
		Maro maro;
		sf::FloatRect hitBox = sf::FloatRect(100, 100, CELL_SIZE, CELL_SIZE);
		REQUIRE(maro.get_x() == 100);
		REQUIRE(maro.get_y() == 100);
		REQUIRE(maro.get_flipped() == 0);
		REQUIRE(maro.get_big() == 0);
		REQUIRE(maro.get_hit_box() == hitBox);
	}


	SECTION("Testing Setters") {
		Maro maro;
		maro.set_position(200, 200);
		REQUIRE(maro.get_x() == 200);
		REQUIRE(maro.get_y() == 200);
		maro.become_big();
		REQUIRE(maro.get_big() == 1);
		maro.become_small();
		REQUIRE(maro.get_big() == 0);
		sf::FloatRect hitBox = sf::FloatRect(200, 200, CELL_SIZE, CELL_SIZE);
		REQUIRE(maro.get_hit_box() == hitBox);


	}
}


TEST_CASE("Testing Roomba") {


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
        mushroom.set_x_y(15, 20);
        CHECK(mushroom.get_x() == 15);
        CHECK(mushroom.get_y() == 20);
    }
}

TEST_CASE("Testing Level Manager") {


    SECTION("testing level 1") {
        Maro maro;
        Roomba roomba1;
        Roomba roomba2;
        std::vector<Roomba> roombas = { roomba1, roomba2 };
        sf::Color backgroundColor = sf::Color(0, 219, 255);
        sf::Image mapSketch;
        mapSketch.loadFromFile("LevelSketch0.png");
        LevelManager levelManager(mapSketch);
        sf::Color pixel = levelManager.get_map_sketch_pixel(16, 9);
		std::cout << pixel.r << ',' << pixel.g << ',' << pixel.b;
		CHECK(unsigned int(pixel.r) == 255);
		CHECK(unsigned int(pixel.g) == 146);
		CHECK(unsigned int(pixel.b) == 85);
    }
}


TEST_CASE("Testing Collisions") {
	// Some stuff needeed for tests
	Maro maro;
	unsigned char levelFinish = 0;
	sf::Color backgroundColor = sf::Color(0, 219, 255);
	sf::Image mapSketch;
	sf::Image mapSketch2;
	mapSketch.loadFromFile("LevelSketch0.png");
	mapSketch2.loadFromFile("LevelSketch1.png");
	sf::Texture mapTexture;
	mapTexture.loadFromFile("Map.png");
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
		Collisions::coin_collision(collisionCells, 43 * CELL_SIZE, 5 * CELL_SIZE, map2, 0, count);
		REQUIRE(collisionCells[1].x == 43);
		REQUIRE(collisionCells[1].y == 5);
	}


	SECTION("question block collision") {
		Collisions::get_collision_question_block(collisionCells, 12 * CELL_SIZE, 9 * CELL_SIZE, map2, 0);
		REQUIRE(collisionCells[0].x == 12);
		REQUIRE(collisionCells[0].y == 9);
	}


	SECTION("brick collision") {
		Collisions::get_collision_brick(collisionCells, 29 * CELL_SIZE, 8 * CELL_SIZE, map2);
		REQUIRE(collisionCells[0].x == 29);
		REQUIRE(collisionCells[0].y == 8);
	}

}