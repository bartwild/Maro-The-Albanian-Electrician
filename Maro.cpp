#include "Maro.h"


unsigned char map_collision(float x, float y, const Map& aMap, bool isBig) { //5-lewo. 10 prawo, 3 gora, 12 dol
	float cellX = x / CELL_SIZE;
	float cellY = y / CELL_SIZE;
	unsigned char output = 0;

	for (unsigned char i = 0; i < 4; i++)
	{
		short x;
		short y;
		switch (i) {
			case 0: {
				x = floor(cellX);
				y = floor(cellY);
				break;
			}
			case 1: {
				x = ceil(cellX);
				y = floor(cellY);
				break;
			}
			case 2: {
				x = floor(cellX);
				y = ceil(cellY);
				if (isBig)	y = ceil(cellY + 1);
				break;
			}
			case 3: {
				x = ceil(cellX);
				y = ceil(cellY);
				if (isBig)	y = ceil(cellY + 1);
			}
		}
		if (x >= 0 && x < aMap.size()){
			if (0 <= y && y < aMap[0].size()){
				if (colisionCell.end() != std::find(colisionCell.begin(), colisionCell.end(), aMap[x][y])) output += pow(2, i);
			}
		}
		else if (colisionCell.end() != std::find(colisionCell.begin(), colisionCell.end(), Cell::Wall)) output += pow(2, i);
	}
	return output;
}


void coin_collision(std::vector<sf::Vector2i>& aCollisionCells, float x, float y, Map& aMap, bool isBig) {
	{
		float cellX = x / CELL_SIZE;
		float cellY = y / CELL_SIZE;
		aCollisionCells.clear();

		for (unsigned char i = 0; i < 4; i++)
		{
			short x;
			short y;
			switch (i) {
			case 0: {
				x = floor(cellX);
				y = floor(cellY);
				break;
			}
			case 1: {
				x = ceil(cellX);
				y = floor(cellY);
				break;
			}
			case 2: {
				x = floor(cellX);
				y = ceil(cellY);
				break;
			}
			case 3: {
				x = ceil(cellX);
				y = ceil(cellY);
			}
			}
			if (x >= 0 && x < aMap.size()) {
				if (0 <= y && y < aMap[0].size()) {
					if (aMap[x][y] == Cell::Coin) {
						aCollisionCells.push_back(sf::Vector2i(x, y));
					}
				}
			}
		}
	}
}



void get_collision_question_block(std::vector<sf::Vector2i>& aCollisionCells, float x, float y, const Map& aMap)
{
	float cellX = x / CELL_SIZE;
	float cellY = y / CELL_SIZE;
	aCollisionCells.clear();

	for (unsigned char i = 0; i < 4; i++)
	{
		short x;
		short y;
		switch (i) {
		case 0: {
			x = floor(cellX);
			y = floor(cellY);
			break;
		}
		case 1: {
			x = ceil(cellX);
			y = floor(cellY);
			break;
		}
		case 2: {
			x = floor(cellX);
			y = ceil(cellY);
			break;
		}
		case 3: {
			x = ceil(cellX);
			y = ceil(cellY);
		}
		}
		if (x >= 0 && x < aMap.size()) {
			if (0 <= y && y < aMap[0].size()) {
				if (aMap[x][y] == Cell::QuestionBlock) {
					aCollisionCells.push_back(sf::Vector2i(x, y));
				}
			}
		}
	}
}


Maro::Maro() {
	flipped = 0;
	onGround = 0;
	x = 100;
	y = 100;
	dead = 0;
	ySpeed = 0;
	xSpeed = 0;
	big = 0;
	hit = 0;
	hitTimer = 0;
	deathTimer = MARO_DEATH_TIMER;
	growthTimer = 0;
	jumpTimer = 0;
	walkAnimation = Animation(CELL_SIZE, MARO_WALK_TEXTURE, MARO_WALK_ANIMATION_SPEED);
	bigWalkAnimation = Animation(CELL_SIZE, "BigMarioWalk.png", MARO_WALK_ANIMATION_SPEED);
	texture.loadFromFile("MarioIdle.png");
	sprite.setTexture(texture);
}


void Maro::draw(sf::RenderWindow& aWindow) {
	bool drawSprite = 1;
	bool drawBig = 0 == growthTimer / MARO_BLINK % 2;
	bool invincible = 0 == hitTimer / MARO_BLINK % 2;
	sprite.setPosition(round(x), round(y));
	if (dead) {
		if (!big) {
			texture.loadFromFile("MarioDeath.png");
		}
		else {
			texture.loadFromFile("BigMarioDeath.png");
		}
	}
	else if (big) {
		if (!onGround) {
			if (!drawBig) {
				sprite.setPosition(round(x), CELL_SIZE + round(y));
				texture.loadFromFile("MarioJump.png");
			}
			else {
				texture.loadFromFile("BigMarioJump.png");
			}
		}
		else {
			if (!xSpeed) {
				if (!drawBig) {
					sprite.setPosition(round(x), CELL_SIZE + round(y));
					texture.loadFromFile("MarioIdle.png");
				}
				else {
					texture.loadFromFile("BigMarioIdle.png");
				}
			}
			else if (((xSpeed > 0 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
				sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
				(xSpeed < 0 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))) {
				if (xSpeed > 0) flipped = 0;
				else flipped = 1;
				if (!drawBig) {
					sprite.setPosition(round(x), CELL_SIZE + round(y));
					texture.loadFromFile("MarioBrake.png");
				}
				else {
					texture.loadFromFile("BigMarioBrake.png");
				}
			}
			else {
				drawSprite = 0;
				if (!drawBig) {
					walkAnimation.set_flipped(flipped);
					walkAnimation.set_position(round(x), round(y));
					walkAnimation.draw(aWindow);
				}
				else {
					bigWalkAnimation.set_flipped(flipped);
					bigWalkAnimation.set_position(round(x), round(y));
					bigWalkAnimation.draw(aWindow);
				}
			}
		}
	}
	else if (invincible) {
		if (!onGround) {
			texture.loadFromFile("MarioJump.png");
		}
		else if (!xSpeed) {
			texture.loadFromFile("MarioIdle.png");
		}
		else if (((xSpeed > 0 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
			sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
			(xSpeed < 0 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
				sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))) {
			if (xSpeed > 0) flipped = 0;
			else flipped = 1;
			texture.loadFromFile("MarioBrake.png");
		}
		else {
			drawSprite = 0;
			walkAnimation.set_flipped(flipped);
			walkAnimation.set_position(round(x), round(y));
			walkAnimation.draw(aWindow);
		}
	}
	if (drawSprite && invincible || drawSprite && dead) {
		if (!flipped) {
			sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(texture.getSize().x), texture.getSize().y));
		}
		else {
			sprite.setTextureRect(sf::IntRect(texture.getSize().x, 0, -static_cast<int>(texture.getSize().x), texture.getSize().y));
		}
		aWindow.draw(sprite);
	}
}


void Maro::draw_mushrooms(const unsigned aViewX, sf::RenderWindow& aWindow){
	for (Mushroom& mushroom : mushrooms){
		mushroom.draw(aViewX, aWindow);
	}
}


void Maro::update(LevelManager& levelManager, unsigned int aViewX, Map& aMap, std::vector<std::shared_ptr<Roomba>> aRoombas){
	for (Mushroom& mushroom : mushrooms){
		mushroom.move(aViewX, aMap);
	}
	bool moving = 0;
	onGround = 0;
	std::vector<sf::Vector2i> cells;
	unsigned char xCollision;
	unsigned char yCollision;
	hit = 0;
	if (!dead){
		moving = xMove(moving);
		if (!big){
			xCollision = map_collision(xSpeed + x, y, aMap, 0);
			if (xCollision != 0){
				moving = 0;
				if (5 & ~xCollision && 10 & xCollision)
				{
					x = CELL_SIZE * (ceil((xSpeed + x) / CELL_SIZE) - 1);
				}
				else if (5 & xCollision && 10 & ~xCollision)
				{
					x = CELL_SIZE * (1 + floor((xSpeed + x) / CELL_SIZE));
				}
				xSpeed = 0;
			}
			yCollision = map_collision(x, 1 + y, aMap, 0);
			yMove(yCollision);
			yCollision = map_collision(x, ySpeed + y, aMap, 0);
			get_collision_question_block(cells, x, y + ySpeed, aMap);
			if (ySpeed <= 0) {
				for (const sf::Vector2i& cell : cells){
					levelManager.set_map_cell(aMap, cell.x, cell.y, Cell::ActivatedQuestionBlock);
					if (levelManager.get_map_sketch_pixel(cell.x, cell.y) == sf::Color(255, 73, 85))
						mushrooms.push_back(Mushroom(CELL_SIZE * cell.x, CELL_SIZE * cell.y));
					else
						levelManager.add_question_block_coin(CELL_SIZE * cell.x, CELL_SIZE * cell.y);
				}
			}
			if (yCollision > 0) {
				if (3 & yCollision && 12 & ~yCollision) {
					y = CELL_SIZE * (1 + floor((ySpeed + y) / CELL_SIZE));
				}
				else if (3 & ~yCollision && 12 & yCollision) {
					y = CELL_SIZE * (ceil((ySpeed + y) / CELL_SIZE) - 1);
				}
				ySpeed = 0;
			}
			yCollision = map_collision(x, 1 + y, aMap, 0);
			if (yCollision > 0) {
				onGround = 1;
			}
			if (y >= SCREEN_HEIGHT - get_hit_box().height)
			{
				die(1);
			}
			coin_collision(cells, x, y, aMap, big);
			for (const sf::Vector2i& cell : cells) {
				levelManager.set_map_cell(aMap, cell.x, cell.y, Cell::Empty);
			}
			check_collision_with_Roombas(aRoombas);
			x += xSpeed;
			y += ySpeed;
		}
		else{
			xCollision = map_collision(xSpeed + x, y, aMap, 1);
			if (xCollision != 0) {
				moving = 0;
				if (5 & ~xCollision && 10 & xCollision)
				{
					x = CELL_SIZE * (ceil((xSpeed + x) / CELL_SIZE) - 1);
				}
				else if (5 & xCollision && 10 & ~xCollision)
				{
					x = CELL_SIZE * (1 + floor((xSpeed + x) / CELL_SIZE));
				}
				xSpeed = 0;
			}
			yCollision = map_collision(x, 1 + y, aMap, 1);
			yMove(yCollision);
			yCollision = map_collision(x, ySpeed + y, aMap, 1);
			get_collision_question_block(cells, x, y + ySpeed, aMap);
			if (ySpeed <= 0) {
				for (const sf::Vector2i& cell : cells){
					levelManager.set_map_cell(aMap, cell.x, cell.y, Cell::ActivatedQuestionBlock);
					if (levelManager.get_map_sketch_pixel(cell.x, cell.y) == sf::Color(255, 73, 85))
						mushrooms.push_back(Mushroom(CELL_SIZE * cell.x, CELL_SIZE * cell.y));
					else
						levelManager.add_question_block_coin(CELL_SIZE * cell.x, CELL_SIZE * cell.y);
				}
			}
			if (yCollision > 0) {
				if (3 & yCollision && 12 & ~yCollision) {
					y = CELL_SIZE * (1 + floor((ySpeed + y) / CELL_SIZE));
				}
				else if (3 & ~yCollision && 12 & yCollision) {
					y = CELL_SIZE * (ceil((ySpeed + y) / CELL_SIZE) - 1);
				}
				ySpeed = 0;
			}
			yCollision = map_collision(x, 1 + y, aMap, 1);
			if (yCollision > 0) {
				onGround = 1;
			}
			if (y >= SCREEN_HEIGHT - get_hit_box().height){
				die(1);
			}
			coin_collision(cells, x, y, aMap, big);
			for (const sf::Vector2i& cell : cells) {
				levelManager.set_map_cell(aMap, cell.x, cell.y, Cell::Empty);
			}
			check_collision_with_Roombas(aRoombas);
			x += xSpeed;
			y += ySpeed;
		}
	}
	else {
		if (deathTimer == 0){
			ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
			y += ySpeed;
		}
		else if (deathTimer == 1){
			ySpeed = MARO_JUMP_SPEED;
		}
		deathTimer = std::max(0, deathTimer - 1);
	}
	check_collision_with_Mushrooms(mushrooms);
	if (big) { bigWalkAnimation.step(1); }
	walkAnimation.step(0);
}


bool Maro::xMove(bool moving){
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
		moving = 1;
		flipped = 0;
		xSpeed = std::min(xSpeed + MARO_ACCELERATION, MARO_SPEED);
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		moving = 1;
		flipped = 1;
		xSpeed = std::max(xSpeed - MARO_ACCELERATION, -MARO_SPEED);
	}
	if (moving == 0){
		if (xSpeed > 0){
			xSpeed = std::max<float>(0, xSpeed - MARO_ACCELERATION);
		}
		else if (xSpeed < 0){
			xSpeed = std::min<float>(0, MARO_ACCELERATION + xSpeed);
		}
	}
	return moving;
}

void Maro::yMove(unsigned char yCollision){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		if (ySpeed == 0 && yCollision > 0) {
			ySpeed = MARO_JUMP_SPEED;
			jumpTimer = MARO_JUMP_TIMER;
		}
		else if (jumpTimer > 0) {
			ySpeed = MARO_JUMP_SPEED;
			jumpTimer--;
		}
		else {
			ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
		}
	}
	else {
		ySpeed = std::min(GRAVITY + ySpeed, MARO_VMAX);
		jumpTimer = 0;
	}
}


float Maro::get_x() const { return x; }


float Maro::get_y() const { return y; }


void Maro::set_position(float newX, float newY){
	x = newX;
	y = newY;
}


void Maro::die(bool instant){
	if (instant) { dead = 1; }
	else {
		if (!big){
			dead = 1;
		}
		else{
			become_small();
		}
	}
}


sf::FloatRect Maro::get_hit_box() const{
	if (!big) {
		return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
	}
	else {
		return sf::FloatRect(x, y, CELL_SIZE, 2 * CELL_SIZE);
	}
}

char Maro::get_death_timer() {
	return deathTimer;
}


void Maro::become_small() {
	big = 0;
}


void Maro::become_big() {
	big = 1;
}


void Maro::reset() {
	dead = 0;
	flipped = 0;
	onGround = 0;
	xSpeed = 0;
	ySpeed = 0;
	x = 0;
	y = 0;
	jumpTimer = 0;
	big = 0;
	deathTimer = MARO_DEATH_TIMER;
	growthTimer = 0;
	mushrooms.clear();
	texture.loadFromFile("MarioIdle.png");
	sprite.setTexture(texture);
	bigWalkAnimation.set_flipped(0);
	walkAnimation.set_flipped(0);
}


void Maro::check_collision_with_Roombas(std::vector<std::shared_ptr<Roomba>> aRoombas){
	std::shared_ptr<Roomba> hitRoomba = nullptr;
	for (std::shared_ptr<Roomba> roomba : aRoombas) {
		if (get_hit_box().intersects(roomba->get_hit_box()) == 1 && roomba->get_whether_dead() == 0){
			hitRoomba = std::move(roomba);
			hit = 1;
			break;
		}
	}
	if (ySpeed > 0 && hit == 1 && hitRoomba->get_ySpeed() == 0 && hitTimer == 0){
		hitRoomba->die();
		ySpeed = MARO_VKILL;
	}
	else if (onGround == 1 && hit == 1 && hitTimer == 0 || ySpeed <= 0 && hit == 1 && hitTimer == 0 && hitRoomba->get_ySpeed() > 0){
		hitTimer = MARO_HIT_TIMER;
		die(0);
	}
	if (hitTimer > 0){
		hitTimer = std::max(0, hitTimer-1);
	}
}

void Maro::check_collision_with_Mushrooms(std::vector<Mushroom>& aMushrooms){
	for (Mushroom& mushroom : mushrooms){
		if (get_hit_box().intersects(mushroom.get_hit_box())){
			mushroom.die(1);
			if (!big){
				growthTimer = MARO_GROWTH_DURATION;
				become_big();
				y -= CELL_SIZE;
			}
		}
	}
	if (0 < growthTimer) {
		growthTimer--;
	}
	mushrooms.erase(remove_if(mushrooms.begin(), mushrooms.end(), [](const Mushroom& i_mushroom){
		return i_mushroom.get_dead() == 1;
	}), mushrooms.end());
}