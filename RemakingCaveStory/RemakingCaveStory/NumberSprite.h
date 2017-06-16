#ifndef NUMBERSPRITE_H_
#define NUMBERSPRITE_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include "Units.h"

class Graphics;
class Sprite;

class NumberSprite 
{
public:
	static NumberSprite HUDNumber(Graphics& graphics, int number, int num_digits)
	{
		return NumberSprite(graphics, number, num_digits, WHITE, NONE);
	}

	static NumberSprite DamageNumber(Graphics& graphics, int number)
	{
		return NumberSprite(graphics, number, 0, RED, MINUS);
	}

	static NumberSprite ExperienceNumber(Graphics& graphics, int number)
	{
		return NumberSprite(graphics, number, 0, WHITE, PLUS);
	}

	void draw(Graphics& graphics, units::Game x, units::Game y);

	units::Game width() const { return (units::kHalfTile * reversed_sprites_.size()); }
	units::Game height() const { return (units::kHalfTile); }
private:
	enum ColorType
	{
		RED,
		WHITE
	};

	enum OperatorType
	{
		PLUS,
		MINUS,
		NONE
	};

	NumberSprite(Graphics& graphics, int number, int num_digits, ColorType color, OperatorType op);

	units::Game padding_;
	std::vector<boost::shared_ptr<Sprite>> reversed_sprites_;
};

#endif // !NUMBERSPRITE_H_
