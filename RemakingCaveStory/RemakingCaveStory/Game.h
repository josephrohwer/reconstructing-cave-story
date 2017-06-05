#pragma once
#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include <boost/scoped_ptr.hpp>
#include "Units.h"

class Player;
class Graphics;
class Map;

class Game {
private:
	boost::scoped_ptr<Player> player_;
	boost::scoped_ptr<Map> map_;
	void eventLoop();
	void update(units::MS elapsed_time_ms);
	void draw(Graphics& graphics);
public:
	Game();
	~Game();
	static int kTileSize;
	static int kScreenWidth;
	static int kScreenHeight;
	template <typename T>
	static T gameUnitsToPixels(T gameUnits);
};

// STATIC
template <typename T>
T Game::gameUnitsToPixels(T gameUnits) {
	return gameUnits * Game::kTileSize / 32;
}

#endif // !GAME_H_
