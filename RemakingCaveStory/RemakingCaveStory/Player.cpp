#include <cmath>
#include <algorithm>
#include <string>
#include "Player.h"
#include "Graphics.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "Map.h"
#include "Rectangle.h"

namespace {
	// WALK MOTION
	const units::Acceleration kWalkingAcceleration = Game::gameUnitsToPixels(0.00083007812f);
	const units::Velocity kMaxSpeedX = Game::gameUnitsToPixels(0.15859375f);
	const units::Acceleration kFriction = Game::gameUnitsToPixels(0.00049804687f);

	// FALL MOTION
	const units::Acceleration kGravity = Game::gameUnitsToPixels(0.00078125f);
	const units::Velocity kMaxSpeedY = Game::gameUnitsToPixels(0.2998046875f);

	// JUMP MOTION
	const units::Velocity kJumpSpeed = Game::gameUnitsToPixels(0.25f);
	const units::Acceleration kAirAcceleration = Game::gameUnitsToPixels(0.0003125f);
	const units::Acceleration kJumpGravity = Game::gameUnitsToPixels(0.0003125f);

	// SPRITES
	const std::string kSpriteFileName = "MyChar";

	// SPRITE FRAMES
	const int kCharacterFrame = 0;
	const int kWalkFrame = 0;
	const int kStandFrame = 0;
	const int kJumpFrame = 1;
	const int kFallFrame = 2;
	const int kUpFrameOffset = 3;
	const int kDownFrame = 6;
	const int kBackFrame = 7;

	// WALK ANIMATION
	const int kNumWalkFrames = 3;
	const units::FPS kWalkFps = 15;

	// COLLLISION RECTANGLE
	const Rectangle kCollisionX(Game::gameUnitsToPixels(6), 
		Game::gameUnitsToPixels(10), 
		Game::gameUnitsToPixels(20), 
		Game::gameUnitsToPixels(12));
	
	const Rectangle kCollisionY(Game::gameUnitsToPixels(10), 
		Game::gameUnitsToPixels(2), 
		Game::gameUnitsToPixels(12), 
		Game::gameUnitsToPixels(30));

	struct CollisionInfo {
		bool collided;
		int row;
		int col;
	};
	CollisionInfo getWallCollisionInfo(const Map& map, const Rectangle& rectangle) {
		CollisionInfo info = { false, 0, 0 };
		std::vector<Map::CollisionTile> tiles(map.getCollidingTiles(rectangle));
		for (size_t i = 0; i < tiles.size(); i++) {
			if (tiles[i].tile_type == Map::WALL_TILE) {
				info = { true, tiles[i].row, tiles[i].col };
				break;
			}
		}
		return info;
	}
}

// Determines which way the player is facing.
bool operator<(const Player::SpriteState& a, const Player::SpriteState& b) {
	if (a.motion_type != b.motion_type) {
		return a.motion_type < b.motion_type;
	}
	if (a.horizontal_facing != b.horizontal_facing) {
		return a.horizontal_facing < b.horizontal_facing;
	}
	if (a.vertical_facing != b.vertical_facing) {
		return a.vertical_facing < b.vertical_facing;
	}
	return false;
}

Player::Player(Graphics& graphics, int x, int y) :
	velocity_x_(0.0f), velocity_y_(0.0f),
	acceleration_x_(0),
	x_(x), y_(y),
	horizontal_facing_(LEFT), vertical_facing_(HORIZONTAL),
	on_ground_(false),
	jump_active_(false),
	interacting_(false)
{
	initializeSprites(graphics);
}

void Player::update(units::MS elapsed_time_ms, const Map& map) {
	sprites_[getSpriteState()]->update(elapsed_time_ms);

	updateX(elapsed_time_ms, map);
	updateY(elapsed_time_ms, map);
}

void Player::draw(Graphics& graphics) {
	sprites_[getSpriteState()]->draw(graphics, x_, y_);
}

void Player::startMovingLeft() {
	acceleration_x_ = -1;
	horizontal_facing_ = LEFT;
	interacting_ = false;
}

void Player::startMovingRight() {
	acceleration_x_ = 1;
	horizontal_facing_ = RIGHT;
	interacting_ = false;
}

void Player::lookUp() {
	vertical_facing_ = UP;
	interacting_ = false;
}

void Player::lookDown() {
	if (vertical_facing_ == DOWN) return; // So this doesn't set vertical facing when player comes down from the air.
	vertical_facing_ = DOWN;
	interacting_ = on_ground();
}

void Player::lookHorizontal() {
	vertical_facing_ = HORIZONTAL;
}

void Player::stopMoving() {
	acceleration_x_ = 0;
}

void Player::startJump() {
	interacting_ = false;
	jump_active_ = true;
	if (on_ground()) { // Player is on the ground.
		velocity_y_ = -kJumpSpeed; // Give player initial velocity up.
	}
}

void Player::stopJump() {
	jump_active_ = false;
}

void Player::initializeSprites(Graphics& graphics) {
	for (int m_type = FIRST_MOTION_TYPE; m_type < LAST_MOTION_TYPE; m_type++) {
		for (int h_facing = FIRST_HORIZONTAL_FACING; h_facing < LAST_HORIZONTAL_FACING; h_facing++) {
			for (int v_facing = FIRST_VERTICAL_FACING; v_facing < LAST_VERTICAL_FACING; v_facing++) {
				MotionType motion_type = static_cast<MotionType>(m_type);
				HorizontalFacing horizontal_facing = static_cast<HorizontalFacing>(h_facing);
				VerticalFacing vertical_facing = static_cast<VerticalFacing>(v_facing);
				initializeSprite(graphics, SpriteState(motion_type, horizontal_facing, vertical_facing));
			}
		}
	}
}

void Player::initializeSprite(Graphics& graphics, const SpriteState& sprite_state) {
	int source_y = sprite_state.horizontal_facing == LEFT ? (kCharacterFrame * Game::kTileSize) : (1 + kCharacterFrame) * Game::kTileSize;
	int source_x;

	switch (sprite_state.motion_type) {
	case WALKING:
		source_x = kWalkFrame * Game::kTileSize;
		break;
	case STANDING:
		source_x = kStandFrame * Game::kTileSize;
		break;
	case INTERACTING:
		source_x = kBackFrame * Game::kTileSize;
		break;
	case JUMPING:
		source_x = kJumpFrame * Game::kTileSize;
		break;
	case FALLING:
		source_x = kFallFrame * Game::kTileSize;
		break;
	case LAST_MOTION_TYPE:
		break;
	}
	source_x = sprite_state.vertical_facing == UP ? source_x + (kUpFrameOffset * Game::kTileSize) : source_x;

	if (sprite_state.motion_type == WALKING) {
		sprites_[sprite_state]
			= boost::shared_ptr<Sprite>(new AnimatedSprite
			(graphics, kSpriteFileName, source_x, source_y, Game::kTileSize, Game::kTileSize, kWalkFps, kNumWalkFrames));
	}
	else {
		if (sprite_state.vertical_facing == DOWN && (sprite_state.motion_type == JUMPING || sprite_state.motion_type == FALLING)) {
			source_x = (kDownFrame * Game::kTileSize);
		}
		sprites_[sprite_state]
			= boost::shared_ptr<Sprite>(new Sprite
			(graphics, kSpriteFileName, source_x, source_y, Game::kTileSize, Game::kTileSize));
	}
}

Player::SpriteState Player::getSpriteState() {
	MotionType motion;
	if (interacting_) {
		motion = INTERACTING;
	}
	else if (on_ground()) {
		motion = (acceleration_x_ == 0) ? STANDING : WALKING;
	}
	else {
		motion = velocity_y_ < 0.0f ? JUMPING : FALLING;
	}
	return SpriteState(motion, horizontal_facing_, vertical_facing_);
}

Rectangle Player::leftCollision(int delta) const {
	assert(delta <= 0);
	return Rectangle(
		x_ + kCollisionX.left() + delta, 
		y_ + kCollisionX.top(), 
		(kCollisionX.width() / 2) - delta,
		(kCollisionX.height() / 2));
}

Rectangle Player::rightCollision(int delta) const {
	assert(delta >= 0);
	return Rectangle(
		x_ + kCollisionX.left() + (kCollisionX.width() / 2), 
		y_ + kCollisionX.top(),
		(kCollisionX.width() / 2) + delta, 
		kCollisionX.height()
	);
}

Rectangle Player::topCollision(int delta) const {
	assert(delta <= 0);
	return Rectangle(
		x_ + kCollisionY.left(),
		y_ + kCollisionY.top() + delta,
		kCollisionY.width(),
		(kCollisionY.height() / 2) - delta
	);
}

Rectangle Player::bottomCollision(int delta) const {
	assert(delta >= 0);
	return Rectangle(
		x_ + kCollisionY.left(),
		y_ + kCollisionY.top() + (kCollisionY.height() / 2),
		kCollisionY.width(),
		(kCollisionY.height() / 2) + delta
	);
}

void Player::updateX(units::MS elapsed_time_ms, const Map& map) {
	// Update Velocity
	units::Acceleration acceleration_x = 0;
	if (acceleration_x_ < 0) acceleration_x = on_ground() ? -kWalkingAcceleration : -kAirAcceleration;
	else if (acceleration_x_ > 0) acceleration_x = on_ground() ? kWalkingAcceleration : kAirAcceleration;

	velocity_x_ += (acceleration_x * elapsed_time_ms);

	if (acceleration_x_ < 0) {
		velocity_x_ = std::max(velocity_x_, -kMaxSpeedX);
	}
	else if (acceleration_x_ > 0) {
		velocity_x_ = std::min(velocity_x_, kMaxSpeedX);
	}
	else if (on_ground()) {
		velocity_x_ = velocity_x_ > 0.0f ? std::max(0.0f, velocity_x_ - (kFriction * elapsed_time_ms)) : 
			std::min(0.0f, velocity_x_ + (kFriction * elapsed_time_ms));
	}

	// Calculate delta
	const int delta = (int)round((velocity_x_ * elapsed_time_ms));
	if (delta > 0) {
		// Check collision in direction of delta.
		CollisionInfo info = getWallCollisionInfo(map, rightCollision(delta));
		// React to collision
		if (info.collided) {
			x_ = (info.col * Game::kTileSize) - kCollisionX.right();
			velocity_x_ = 0.0f;
		}
		else {
			x_ += delta;
		}

		// Check collision in other direction.
		info = getWallCollisionInfo(map, leftCollision(0));
		// React to collision
		if (info.collided) {
			x_ = (info.col * Game::kTileSize) + kCollisionX.right();
		}
	}
	else {
		// Check collision in direction of delta.
		CollisionInfo info = getWallCollisionInfo(map, leftCollision(delta));
		// React to collision
		if (info.collided) {
			x_ = (info.col * Game::kTileSize) + kCollisionX.right();
			velocity_x_ = 0.0f;
		}
		else {
			x_ += delta;
		}

		// Check collision in other direction.
		info = getWallCollisionInfo(map, rightCollision(0));
		// React to collision
		if (info.collided) {
			x_ = (info.col * Game::kTileSize) - kCollisionX.right();
		}
	}
}

void Player::updateY(units::MS elapsed_time_ms, const Map& map) {
	// Update velocity.
	const units::Acceleration gravity = (jump_active_ && velocity_y_ < 0.0f) ? kJumpGravity : kGravity;
	velocity_y_ = std::min(velocity_y_ + (gravity * elapsed_time_ms), kMaxSpeedY);
	// Calculate delta.
	const int delta = (int)round((velocity_y_ * elapsed_time_ms));
	if (delta > 0) {
		// Check collision in direction of delta.
		CollisionInfo info = getWallCollisionInfo(map, bottomCollision(delta));
		// React to collision
		if (info.collided) {
			y_ = (info.row * Game::kTileSize) - kCollisionY.bottom();
			velocity_y_ = 0.0f;
			on_ground_ = true;
		}
		else {
			y_ += delta;
			on_ground_ = false;
		}

		// Check collision in other direction.
		info = getWallCollisionInfo(map, topCollision(0));
		// React to collision
		if (info.collided) {
			y_ = (info.row * Game::kTileSize) + kCollisionY.height();
		}
	}
	else {
		// Check collision in direction of delta.
		CollisionInfo info = getWallCollisionInfo(map, topCollision(delta));
		// React to collision
		if (info.collided) {
			y_ = (info.row * Game::kTileSize) + kCollisionY.height();
			velocity_y_ = 0.0f;
		}
		else {
			y_ += delta;
			on_ground_ = false;
		}

		// Check collision in other direction.
		info = getWallCollisionInfo(map, bottomCollision(0));
		// React to collision
		if (info.collided) {
			y_ = (info.row * Game::kTileSize) - kCollisionY.bottom();
			on_ground_ = true;
		}
	}
}