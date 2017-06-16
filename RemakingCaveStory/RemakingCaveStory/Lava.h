#ifndef LAVA_H_
#define LAVA_H_

#include <boost\shared_ptr.hpp>
#include <map>
#include "Units.h"
#include "Rectangle.h"

class Graphics;
class Audio;
class Sprite;

class Lava
{
public:
	Lava(Graphics& graphics, units::Game x, units::Game y);
	void update(units::MS elapsed_time);
	void draw(Graphics& graphics) const;

	Rectangle damageRectangle() const { return Rectangle(x_ + units::kHalfTile, y_ + units::kHalfTile, 640, 0); }
private:
	void initializeSprite(Graphics& graphics);

	units::Game x_;
	units::Game y_;
	bool isTopLava_;
	boost::shared_ptr<Sprite> sprite_;
};

#endif // !LAVA_H_
