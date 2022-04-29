#pragma once
#include <vector>
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const char CELL_SIZE = 16;
const char SCREEN_RESIZE = 2;
const float GRAVITY = 0.0001;
const float MARO_ACCELERATION = 0.0002;
const float MARO_SPEED = 0.05 ;
const float MARO_JUMP_SPEED = -0.12;
const float MARO_VMAX = 0.25;
const unsigned char MARO_JUMP_TIMER = 8;

enum Cell {
	Empty,
	Brick,
	Pipe,
	QuestionBlock,
	Wall
};

const std::vector<Cell> colisionCell = { Brick, Pipe, QuestionBlock, Wall };
typedef std::vector <std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>> Map;