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
	NumberSprite(Graphics& graphics, int number, int num_digits = 0);

	void draw(Graphics& graphics, units::Game x, units::Game y);
private:
	units::Game padding_;
	std::vector<boost::shared_ptr<Sprite>> reversed_digits_;
};

#endif // !NUMBERSPRITE_H_
