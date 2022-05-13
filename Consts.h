#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <vector>
#include <chrono>
#include <array>
#include <chrono>
#include <cmath>
#include <algorithm>

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const char MARO_DEATH_TIMER = 64;
const char CELL_SIZE = 16;
const char SCREEN_RESIZE = 4;
const float GRAVITY = 0.25f;
const float MARO_ACCELERATION = 0.1f;
const unsigned short MARO_WALK_ANIMATION_SPEED = 5;
const unsigned short QUESTION_BLOCK_ANIMATION_SPEED = 10;
const float MARO_SPEED = 2 ;
const float MARO_JUMP_SPEED = -4;
const float MARO_VMAX = 8;
const unsigned char MARO_JUMP_TIMER = 9;
const unsigned char MARO_HIT_TIMER = 128;
const float ROOMBA_SPEED = 1;
const float MUSHROOM_SPEED = 1;
const float UPDATE_AREA = 64;
const std::chrono::microseconds FRAME_DURATION(16667);
const std::string MARO_WALK_TEXTURE = "MarioWalk.png";


enum Cell {
	Empty,
	Brick,
	Pipe,
	QuestionBlock,
	Wall,
	ActivatedQuestionBlock
};



const std::vector<Cell> colisionCell = { Brick, Pipe, QuestionBlock, Wall, ActivatedQuestionBlock };
typedef std::vector <std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>> Map;