#ifndef BAT_H_
#define BAT_H_

#include <boost\shared_ptr.hpp>
#include <map>
#include "Units.h"
#include "Rectangle.h"

class Graphics;
class Audio;
class Sprite;

class Bat {
public:
	Bat(Graphics& graphics, units::Game x, units::Game y);
	void update(units::MS elapsed_time, units::Game player_x);
	void draw(Graphics& graphics) const;

	Rectangle damageRectangle() const { return Rectangle(x_ + (units::tileToGame(1) / 2.0f), y_ + (units::tileToGame(1) / 2.0f), 0, 0); }
private:
	enum Facing
	{
		FIRST_FACING,
		LEFT = FIRST_FACING,
		RIGHT,
		LAST_FACING
	};

	struct SpriteState
	{
		SpriteState(Facing facing) : facing(facing)
		{
		}
	
		Facing facing;
	};

	friend bool operator<(const SpriteState& a, const SpriteState& b) { return a.facing < b.facing; }

	void initializeSprites(Graphics& graphics);
	void initializeSprite(Graphics& graphics, const SpriteState& sprite);
	SpriteState getSpriteState() const;

	const units::Game center_y_;
	units::Game x_;
	units::Game y_;
	units::Degrees flight_angle_;
	Facing facing_;
	std::map<SpriteState, boost::shared_ptr<Sprite>> sprites_;
};

#endif // !BAT_H_
