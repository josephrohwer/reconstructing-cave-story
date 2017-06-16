#include <string>
#include "Player.h"

namespace 
{
	// HUD
	const units::Game kHealthBarX = units::tileToGame(2);
	const units::Game kHealthBarY = units::tileToGame(2);
	const units::Game kHealthBarSourceX = 0;
	const units::Game kHealthBarSourceY = (5 * units::kHalfTile);
	const units::Game kHealthBarSourceWidth = units::tileToGame(4);
	const units::Game kHealthBarSourceHeight = units::kHalfTile;

	const units::Game kHealthDamageSourceX = 0;
	const units::Game kHealthDamageSourceY = units::tileToGame(2);
	const units::Game kHealthDamageSourceHeight = units::kHalfTile;

	const units::Game kMaxFillWidth = (5 * units::kHalfTile) - 2.0f;

	const units::Game kHealthFillX = (7 * units::kHalfTile);
	const units::Game kHealthFillY = units::tileToGame(2);
	const units::Game kHealthFillSourceX = 0;
	const units::Game kHealthFillSourceY = (3 * units::kHalfTile);
	const units::Game kHealthFillSourceHeight = units::kHalfTile;

	const units::Game kHealthNumberX = (units::tileToGame(5) / 2);
	const units::Game kHealthNumberY = units::tileToGame(2);
	const int kHealthNumberNumDigits = 2;

	const std::string kSpritePath("TextBox");

	const units::MS kDamageDelay = 1000;
}

Player::Health::Health(Graphics& graphics) :
	damage_(0),
	damage_timer_(kDamageDelay),
	max_health_(6),
	current_health_(6),
	health_bar_sprite_(
		graphics, kSpritePath,
		units::gameToPixel(kHealthBarSourceX),
		units::gameToPixel(kHealthBarSourceY),
		units::gameToPixel(kHealthBarSourceWidth),
		units::gameToPixel(kHealthBarSourceHeight)),
	
	health_fill_sprite_(
		graphics, kSpritePath,
		units::gameToPixel(kHealthFillSourceX),
		units::gameToPixel(kHealthFillSourceY),
		units::gameToPixel(kMaxFillWidth),
		units::gameToPixel(kHealthBarSourceHeight)),

	damage_fill_sprite_(
		graphics, kSpritePath,
		units::gameToPixel(kHealthDamageSourceX),
		units::gameToPixel(kHealthDamageSourceY),
		units::gameToPixel(0),
		units::gameToPixel(kHealthDamageSourceHeight))
{
}

void Player::Health::update(units::MS elapsed_time)
{
	if (damage_ > 0 && damage_timer_.expired() && damage_ <= current_health_)
	{
		current_health_ -= damage_;
		damage_ = 0;
	}
}

void Player::Health::draw(Graphics& graphics)
{
	health_bar_sprite_.draw(graphics, kHealthBarX, kHealthBarY);
	health_fill_sprite_.draw(graphics, kHealthFillX, kHealthFillY);

	if (damage_ > 0 && damage_ <= current_health_)
	{
		damage_fill_sprite_.draw(graphics, kHealthFillX + fillOffset(current_health_ - damage_), kHealthFillY);
	}

	NumberSprite::HUDNumber(graphics, current_health_, kHealthNumberNumDigits).draw(graphics, kHealthNumberX, kHealthNumberY);
}

bool Player::Health::takeDamage(units::HP damage)
{
	damage_ = damage;
	damage_timer_.reset();
	health_fill_sprite_.setWidth(units::gameToPixel(fillOffset(current_health_ - damage)));
	damage_fill_sprite_.setWidth(units::gameToPixel(fillOffset(damage)));
	return false;
}

units::Game Player::Health::fillOffset(units::HP health) const
{
	return kMaxFillWidth * health / max_health_;
}