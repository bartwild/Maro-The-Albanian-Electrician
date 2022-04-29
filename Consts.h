#pragma once
#include <vector>
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const char CELL_SIZE = 16;
const char SCREEN_RESIZE = 2;
enum Cell {
	Empty,
	Wall
};
typedef std::vector <std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>>Map;