#include "Bat.h"
#include "AnimatedSprite.h"
#include "Graphics.h"
#include "Audio.h"

namespace {
	const units::Frame kNumFlyFrames = 3;
	const units::FPS kFlyFps = 13;
	const units::AngularVelocity kAngularVelocity = (120.0f / 1000.0f);
}

Bat::Bat(Graphics& graphics, units::Game x, units::Game y) : 
	center_y_(y),
	x_(x), 
	y_(y),
	facing_(RIGHT),
	flight_angle_(0.0f)
{
	initializeSprites(graphics);
}

void Bat::update(units::MS elapsed_time, units::Game player_x)
{
	flight_angle_ += (kAngularVelocity * elapsed_time);

	facing_ = (x_ + (units::tileToGame(1) / 2.0f) > player_x) ? LEFT : RIGHT;

	y_ = center_y_ + (units::tileToGame(5) / 2.0f) * (units::Game)std::sin(units::degreesToRadians(flight_angle_));

	sprites_[getSpriteState()]->update(elapsed_time);
}

void Bat::draw(Graphics& graphics) const
{
	sprites_.at(getSpriteState())->draw(graphics, x_, y_);
}

void Bat::initializeSprites(Graphics& graphics)
{
	for (int facing = FIRST_FACING; facing < LAST_FACING; facing++)
	{
		initializeSprite(graphics, SpriteState(Facing(facing)));
	}
}

void Bat::initializeSprite(Graphics& graphics, const SpriteState& sprite_state)
{
	units::Tile tile_y = sprite_state.facing == RIGHT ? 3 : 2;
	sprites_[sprite_state] = boost::shared_ptr<Sprite>(new AnimatedSprite(graphics, "NpcCemet",
		units::tileToPixel(2), units::tileToPixel(tile_y),
		units::tileToPixel(1), units::tileToPixel(1), kFlyFps, kNumFlyFrames));
}

Bat::SpriteState Bat::getSpriteState() const
{
	return SpriteState(facing_);
}