#pragma once
#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <boost/shared_ptr.hpp>

class Sprite;
class Graphics;
class Rectangle;

class Map {
public:
	enum TileType {
		AIR_TILE,
		WALL_TILE
	};

	struct CollisionTile {
		CollisionTile(int row, int col, TileType tile_type) :
			row(row), col(col), tile_type(tile_type) {}
		int row;
		int col;
		TileType tile_type;
	};

	static Map* createTestMap(Graphics& graphics);
	std::vector<CollisionTile> getCollidingTiles(const Rectangle& rectangle) const;
	void update(int elapsed_time_ms);
	void draw(Graphics& graphics) const;
private:
	struct Tile {
		Tile(TileType tile_type = AIR_TILE, boost::shared_ptr<Sprite> sprite = boost::shared_ptr<Sprite>()) :
			tile_type(tile_type), sprite(sprite) {}

		TileType tile_type;
		boost::shared_ptr<Sprite> sprite;
	};

	std::vector<std::vector<Tile>> tiles_;
};

#endif // !MAP_H_
