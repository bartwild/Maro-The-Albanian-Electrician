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
const float COIN_JUMP_SPEED = -6;
const float MARO_SPEED = 2 ;
const float MARO_JUMP_SPEED = -4;
const float MARO_VMAX = 8;
const float MARO_VKILL = -2.5;
const unsigned char MARO_JUMP_TIMER = 9;
const unsigned char MARO_HIT_TIMER = 128;
const unsigned char MARO_BLINK = 4;
const unsigned char MARO_GROWTH_DURATION = 64;
const float ROOMBA_SPEED = 1;
const float MUSHROOM_SPEED = 1;
const float UPDATE_AREA = 64;
const std::chrono::microseconds FRAME_DURATION(16667);
const std::string TEXTURES_PATH = "textures/";
const std::string MARO_WALK_TEXTURE = TEXTURES_PATH + "MaroWalk.png";

enum Cell {
	Empty,
	Brick,
	Pipe,
	QuestionBlock,
	Coin,
	Wall,
	ActivatedQuestionBlock
};


struct Object {
	float xSpeed;
	float ySpeed;
	float x;
	float y;

	Object(const float aX, const float aY, const float aXSpeed = 0, const float aYSpeed = 0) {
		xSpeed = aXSpeed;
		ySpeed = aYSpeed;
		x = aX;
		y = aY;
	}
};



const std::vector<Cell> colisionCell = { Brick, Pipe, QuestionBlock, Wall, ActivatedQuestionBlock };
typedef std::vector <std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>> Map;


const std::vector<sf::Keyboard::Key> keysNoEscAndEnt = { sf::Keyboard::Unknown, sf::Keyboard::A, sf::Keyboard::B,sf::Keyboard::C, sf::Keyboard::D, sf::Keyboard::E, sf::Keyboard::F, sf::Keyboard::G, sf::Keyboard::H,sf::Keyboard::I, sf::Keyboard::J,
sf::Keyboard::K,sf::Keyboard::L, sf::Keyboard::M, sf::Keyboard::N,sf::Keyboard::O, sf::Keyboard::P, sf::Keyboard::Q, sf::Keyboard::R, sf::Keyboard::S, sf::Keyboard::T,sf::Keyboard::U, sf::Keyboard::V, sf::Keyboard::W, sf::Keyboard::X, sf::Keyboard::Y,
 sf::Keyboard::Z, sf::Keyboard::Num0, sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3,  sf::Keyboard::Num4, sf::Keyboard::Num5,sf::Keyboard::Num6, sf::Keyboard::Num7, sf::Keyboard::Num8,sf::Keyboard::Num9,sf::Keyboard::LControl, sf::Keyboard::LShift,
 sf::Keyboard::LAlt,sf::Keyboard::LSystem, sf::Keyboard::RControl,sf::Keyboard::RShift, sf::Keyboard::RAlt, sf::Keyboard::RSystem, sf::Keyboard::Menu, sf::Keyboard::LBracket,sf::Keyboard::RBracket,sf::Keyboard::Semicolon, sf::Keyboard::Comma,sf::Keyboard::Period,
 sf::Keyboard::Quote,sf::Keyboard::Slash, sf::Keyboard::Backslash,sf::Keyboard::Tilde, sf::Keyboard::Equal, sf::Keyboard::Hyphen,  sf::Keyboard::Space, sf::Keyboard::Backspace, sf::Keyboard::Tab, sf::Keyboard::PageUp, sf::Keyboard::PageDown,sf::Keyboard::End,
 sf::Keyboard::Home, sf::Keyboard::Insert, sf::Keyboard::Delete, sf::Keyboard::Add,sf::Keyboard::Subtract, sf::Keyboard::Multiply, sf::Keyboard::Divide, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Numpad0,
sf::Keyboard::Numpad1, sf::Keyboard::Numpad2,sf::Keyboard::Numpad3, sf::Keyboard::Numpad4, sf::Keyboard::Numpad5, sf::Keyboard::Numpad6,sf::Keyboard::Numpad7, sf::Keyboard::Numpad8,sf::Keyboard::Numpad9,sf::Keyboard::F1, sf::Keyboard::F2, sf::Keyboard::F3,
sf::Keyboard::F4, sf::Keyboard::F5,sf::Keyboard::F6,sf::Keyboard::F7,sf::Keyboard::F8, sf::Keyboard::F9, sf::Keyboard::F10, sf::Keyboard::F11, sf::Keyboard::F12, sf::Keyboard::F13, sf::Keyboard::F14,  sf::Keyboard::F15, sf::Keyboard::Pause,
 sf::Keyboard::KeyCount,  sf::Keyboard::Dash, sf::Keyboard::BackSpace,  sf::Keyboard::BackSlash, sf::Keyboard::SemiColon, sf::Keyboard::Return };