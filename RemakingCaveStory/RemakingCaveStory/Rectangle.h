#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Units.h"

class Rectangle
{
public: 
	Rectangle(units::Game x, units::Game y, 
		      units::Game width, units::Game height) :
			  x_(x), y_(y), 
			  width_(width), height_(height) {}

	units::Game left() const { return x_; }
	units::Game right() const { return x_ + width_; }
	units::Game top() const { return y_; }
	units::Game bottom() const { return y_ + height_; }

	units::Game width() const { return width_; }
	units::Game height() const { return height_; }
private:
	const units::Game x_;
	const units::Game y_;
	const units::Game width_;
	const units::Game height_;
};

#endif // !RECTANGLE_H_
