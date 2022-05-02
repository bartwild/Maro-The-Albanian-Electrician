#pragma once
#include <vector>
#include <chrono>
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const char CELL_SIZE = 16;
const char SCREEN_RESIZE = 2;
const float GRAVITY = 0.25f;
const float MARO_ACCELERATION = 0.25f;
const float MARO_SPEED = 2 ;
const float MARO_JUMP_SPEED = -4;
const float MARO_VMAX = 8;
const unsigned char MARO_JUMP_TIMER = 8;
const std::chrono::microseconds FRAME_DURATION(16667);


const enum Cell {
	Empty,
	Brick,
	Pipe,
	QuestionBlock,
	Wall
};

const std::vector<Cell> colisionCell = { Brick, Pipe, QuestionBlock, Wall };
typedef std::vector <std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>> Map;