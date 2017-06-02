#pragma once
#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include <boost/scoped_ptr.hpp>

class Player;
class Graphics;
class Map;

class Game {
private:
	boost::scoped_ptr<Player> player_;
	boost::scoped_ptr<Map> map_;
	void eventLoop();
	void update(int elapsed_time_ms);
	void draw(Graphics& graphics);
public:
	Game();
	~Game();
	static int kTileSize;
};

#endif // !GAME_H_
