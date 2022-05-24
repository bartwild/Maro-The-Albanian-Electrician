#include "Maro.h"


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
	bigWalkAnimation = Animation(CELL_SIZE, TEXTURES_PATH + "BigMaroWalk.png", MARO_WALK_ANIMATION_SPEED);
	texture.loadFromFile(TEXTURES_PATH + "MaroIdle.png");
	sprite.setTexture(texture);
}


void Maro::draw(sf::RenderWindow& aWindow) {
	bool drawSprite = 1;
	bool drawBig = 0 == growthTimer / MARO_BLINK % 2;
	bool invincible = 0 == hitTimer / MARO_BLINK % 2;
	sprite.setPosition(round(x), round(y));
	if (dead) {
		if (!big) {
			texture.loadFromFile(TEXTURES_PATH + "MaroDeath.png");
		}
		else {
			texture.loadFromFile(TEXTURES_PATH + "BigMaroDeath.png");
		}
	}
	else if (big) {
		if (!onGround) {
			if (!drawBig) {
				sprite.setPosition(round(x), CELL_SIZE + round(y));
				texture.loadFromFile(TEXTURES_PATH + "MaroJump.png");
			}
			else {
				texture.loadFromFile(TEXTURES_PATH + "BigMaroJump.png");
			}
		}
		else {
			if (!xSpeed) {
				if (!drawBig) {
					sprite.setPosition(round(x), CELL_SIZE + round(y));
					texture.loadFromFile(TEXTURES_PATH + "MaroIdle.png");
				}
				else {
					texture.loadFromFile(TEXTURES_PATH + "BigMaroIdle.png");
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
					texture.loadFromFile(TEXTURES_PATH + "MaroBrake.png");
				}
				else {
					texture.loadFromFile(TEXTURES_PATH + "BigMaroBrake.png");
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
			texture.loadFromFile(TEXTURES_PATH + "MaroJump.png");
		}
		else if (!xSpeed) {
			texture.loadFromFile(TEXTURES_PATH + "MaroIdle.png");
		}
		else if (((xSpeed > 0 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
			sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
			(xSpeed < 0 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
				sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))) {
			if (xSpeed > 0) flipped = 0;
			else flipped = 1;
			texture.loadFromFile(TEXTURES_PATH + "MaroBrake.png");
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


void Maro::update(LevelManager& levelManager, unsigned int aViewX, Map& aMap, std::vector<std::shared_ptr<Roomba>> aRoombas, unsigned int& count) {
	for (Mushroom& mushroom : mushrooms) {
		mushroom.move(aViewX, aMap);
	}
	bool moving = 0;
	onGround = 0;
	std::vector<sf::Vector2i> cells;
	unsigned char xCollision;
	unsigned char yCollision;
	hit = 0;
	if (!dead) {
		moving = x_move(moving);
		xCollision = Collisions::map_collision(xSpeed + x, y, aMap, big);
		if (xCollision != 0) {
			set_x_after_collision(moving, xCollision);
		}
		yCollision = Collisions::map_collision(x, 1 + y, aMap, big);
		y_move(yCollision);
		if (big) {
			Collisions::get_collision_brick(cells, x, y + ySpeed, aMap);
			if (ySpeed <= 0) {
				for (const sf::Vector2i& cell : cells) {
					levelManager.set_map_cell(aMap, cell.x, cell.y, Cell::Empty);
				}
			}
		}
		yCollision = Collisions::map_collision(x, ySpeed + y, aMap, big);
		Collisions::get_collision_question_block(cells, x, y + ySpeed, aMap);
		if (ySpeed <= 0) {
			question_block_interaction(cells, levelManager, aMap, count);
		}
		if (yCollision > 0) {
			set_y_after_collision(yCollision);
		}
		yCollision = Collisions::map_collision(x, 1 + y, aMap, big);
		if (yCollision > 0) {
			onGround = 1;
		}
		if (y >= SCREEN_HEIGHT - get_hit_box().height)
		{
			die(1);
		}
		Collisions::coin_collision(cells, x, y, aMap, big, count);
		for (const sf::Vector2i& cell : cells) {
			levelManager.set_map_cell(aMap, cell.x, cell.y, Cell::Empty);
		}
		check_collision_with_Roombas(aRoombas, count);
		x += xSpeed;
		y += ySpeed;
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
	check_collision_with_Mushrooms(mushrooms, count);
	if (big) {
		bigWalkAnimation.step(1);
	}
	walkAnimation.step(0);
}


void Maro::set_x_after_collision(bool& moving, unsigned char& xCollision) {
	moving = 0;
	if (5 & ~xCollision && 10 & xCollision) {
		x = CELL_SIZE * (ceil((xSpeed + x) / CELL_SIZE) - 1);
	}
	else if (5 & xCollision && 10 & ~xCollision) {
		x = CELL_SIZE * (1 + floor((xSpeed + x) / CELL_SIZE));
	}
	xSpeed = 0;
}


void Maro::set_y_after_collision(unsigned char& yCollision) {
	if (3 & yCollision && 12 & ~yCollision) {
		y = CELL_SIZE * (1 + floor((ySpeed + y) / CELL_SIZE));
	}
	else if (3 & ~yCollision && 12 & yCollision) {
		y = CELL_SIZE * (ceil((ySpeed + y) / CELL_SIZE) - 1);
	}
	ySpeed = 0;
}


void Maro::question_block_interaction(std::vector<sf::Vector2i>& cells, LevelManager& levelManager, Map& aMap, unsigned int& count) {
	for (const sf::Vector2i& cell : cells) {
		levelManager.set_map_cell(aMap, cell.x, cell.y, Cell::ActivatedQuestionBlock);
		if (levelManager.get_map_sketch_pixel(cell.x, cell.y) == sf::Color(255, 73, 85)) {
			mushrooms.push_back(Mushroom(CELL_SIZE * cell.x, CELL_SIZE * cell.y));
		}
		else {
			levelManager.add_question_block_coin(CELL_SIZE * cell.x, CELL_SIZE * cell.y);
			count += 200;
		}
	}
}


bool Maro::x_move(bool moving) {
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
		!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		moving = 1;
		flipped = 0;
		xSpeed = std::min(xSpeed + MARO_ACCELERATION, MARO_SPEED);
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
		!sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		moving = 1;
		flipped = 1;
		xSpeed = std::max(xSpeed - MARO_ACCELERATION, -MARO_SPEED);
	}
	if (moving == 0) {
		if (xSpeed > 0){
			xSpeed = std::max<float>(0, xSpeed - MARO_ACCELERATION);
		}
		else if (xSpeed < 0) {
			xSpeed = std::min<float>(0, MARO_ACCELERATION + xSpeed);
		}
	}
	return moving;
}


void Maro::y_move(unsigned char yCollision) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)||
		sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
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


float Maro::get_x() const {
	return x;
}


float Maro::get_y() const {
	return y;
}


void Maro::set_position(float newX, float newY) {
	x = newX;
	y = newY;
}


void Maro::die(bool instant) {
	if (instant) {
		dead = 1;
	}
	else {
		if (!big) {
			dead = 1;
		}
		else {
			become_small();
		}
	}
}


sf::FloatRect Maro::get_hit_box() const {
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
	texture.loadFromFile(TEXTURES_PATH + "MaroIdle.png");
	sprite.setTexture(texture);
	bigWalkAnimation.set_flipped(0);
	walkAnimation.set_flipped(0);
}


void Maro::check_collision_with_Roombas(std::vector<std::shared_ptr<Roomba>> aRoombas, unsigned int& count) {
	std::shared_ptr<Roomba> hitRoomba = nullptr;
	for (std::shared_ptr<Roomba> roomba : aRoombas) {
		if (get_hit_box().intersects(roomba->get_hit_box()) == 1 && roomba->get_whether_dead() == 0) {
			hitRoomba = std::move(roomba);
			hit = 1;
			break;
		}
	}
	if (ySpeed > 0 && hit == 1 && hitRoomba->get_ySpeed() == 0 && hitTimer == 0) {
		hitRoomba->die(0);
		count += 200;;
		ySpeed = MARO_VKILL;
	}
	else if (onGround == 1 && hit == 1 && hitTimer == 0 || ySpeed <= 0 && hit == 1 && hitTimer == 0 && hitRoomba->get_ySpeed() > 0) {
		hitTimer = MARO_HIT_TIMER;
		die(0);
	}
	if (hitTimer > 0) {
		hitTimer = std::max(0, hitTimer-1);
	}
}


void Maro::check_collision_with_Mushrooms(std::vector<Mushroom>& aMushrooms, unsigned int& count) {
	for (Mushroom& mushroom : mushrooms) {
		if (get_hit_box().intersects(mushroom.get_hit_box())) {
			mushroom.die(1);
			if (big) count += 1000;
			if (!big) {
				count += 200;
				growthTimer = MARO_GROWTH_DURATION;
				become_big();
				y -= CELL_SIZE;
			}
		}
	}
	if (0 < growthTimer) {
		growthTimer--;
	}
	mushrooms.erase(remove_if(mushrooms.begin(), mushrooms.end(), [](const Mushroom& i_mushroom) {
		return i_mushroom.get_dead() == 1;
	}), mushrooms.end());
}