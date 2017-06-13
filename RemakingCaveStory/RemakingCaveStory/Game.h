#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include <boost/scoped_ptr.hpp>
#include "Units.h"

class Player;
class Bat;
class Graphics;
class Map;

class Game
{
public:
	Game();
	~Game();
	void eventLoop();

	static units::Tile kScreenWidth;
	static units::Tile kScreenHeight;
private:
	void update(units::MS elapsed_time_ms);
	void draw(Graphics& graphics);
	boost::scoped_ptr<Player> player_;
	boost::scoped_ptr<Bat> bat_;
	boost::scoped_ptr<Map> map_;
};

#endif // !GAME_H_
