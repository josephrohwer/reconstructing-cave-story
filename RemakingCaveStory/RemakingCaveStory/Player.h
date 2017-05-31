#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_

#include <boost/shared_ptr.hpp>
#include <map>

class Sprite;
class Graphics;

class Player {
private:
	enum MotionType {
		STANDING,
		WALKING
	};

	enum HorizontalFacing {
		LEFT,
		RIGHT
	};

	struct SpriteState {
		SpriteState(MotionType motion_type = STANDING,
			HorizontalFacing horizontal_facing = LEFT) :
			motion_type(motion_type), horizontal_facing(horizontal_facing) {}
		MotionType motion_type;
		HorizontalFacing horizontal_facing;
	};
	friend bool operator<(const SpriteState& a, const SpriteState& b);
	void initializeSprites(Graphics& graphics);
	SpriteState getSpriteState();
	std::map<SpriteState, boost::shared_ptr<Sprite>> sprites_;
	int x_;
	int y_;
	float velocity_x_;
	float acceleration_x_;
	HorizontalFacing horizontal_facing_;
public:
	Player(Graphics& graphics, int x, int y);
	void update(int elapsed_time_ms);
	void draw(Graphics& graphics);
	void startMovingLeft();
	void startMovingRight();
	void stopMoving();
};

#endif // !PLAYER_H_
