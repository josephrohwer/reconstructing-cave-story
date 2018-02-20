#include "Map.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Game.h"
#include "Rectangle.h"

using boost::shared_ptr;
using std::vector;

// TODO: Implement map loading.

// STATIC
Map* Map::createTestMap(Graphics& graphics)
{
	Map* map = new Map();

	map->backdrop_.reset(new FixedBackdrop("bkBlue", graphics));
	const units::Tile num_rows = 15; // 15 * 32 = 480; SCREEN HEIGHT
	const units::Tile num_cols = 20; // 20 * 32 = 640; SCREEN WIDTH

	// Ensure tiles_ and background_tiles_ are num_rows by num_cols in size.
	map->tiles_ = vector<vector<Tile>>(num_rows, vector<Tile>(num_cols, Tile()));
	map->background_tiles_ = vector<vector<shared_ptr<Sprite>>>(num_rows, vector<shared_ptr<Sprite>>(num_cols, shared_ptr<Sprite>()));
	map->foreground_tiles_ = vector<vector<shared_ptr<Sprite>>>(num_rows, vector<shared_ptr<Sprite>>(num_cols, shared_ptr<Sprite>()));


	shared_ptr<Sprite> lava_top(new Sprite(graphics, "PrtCave",
		units::tileToPixel(14), units::tileToPixel(0), units::tileToPixel(1), units::tileToPixel(1)));

	shared_ptr<Sprite> lava_bottom(new Sprite(graphics, "PrtCave",
		units::tileToPixel(14), units::tileToPixel(1), units::tileToPixel(1), units::tileToPixel(1)));
	Tile LavaTile(WALL_TILE, lava_bottom);

	const units::Tile top = 12;
	const units::Tile bottom = 13;

	for (units::Tile col = 0; col < num_cols; col++)
	{
		map->foreground_tiles_[top][col] = lava_top;
		map->tiles_[bottom][col] = LavaTile;
	}

	shared_ptr<Sprite> Wall1Sprite(new Sprite(graphics, "PrtCave", units::tileToPixel(1), units::tileToPixel(0), units::tileToPixel(1), units::tileToPixel(1)));
	Tile Wall1Tile(WALL_TILE, Wall1Sprite);

	shared_ptr<Sprite> Wall2Sprite(new Sprite(graphics, "PrtCave", units::tileToPixel(9), units::tileToPixel(2), units::tileToPixel(1), units::tileToPixel(1)));
	Tile Wall2Tile(WALL_TILE, Wall2Sprite);

	const units::Tile floor = 14;
	const units::Tile ceiling = 0;
	for (units::Tile col = 0; col < num_cols; col++)
	{
		map->tiles_[floor][col] = Wall2Tile;
		map->tiles_[ceiling][col] = Wall2Tile;
	}

	const units::Tile leftWall = 0;
	const units::Tile rightWall = 19;
	for (units::Tile row = 1; row < floor; row++)
	{
		map->tiles_[row][leftWall] = Wall1Tile;
		map->tiles_[row][rightWall] = Wall1Tile;
	}
	map->tiles_[12][14] = Wall1Tile;
	map->tiles_[6][18] = Wall1Tile;
	map->tiles_[6][17] = Wall1Tile;
	map->tiles_[6][16] = Wall1Tile;
	map->tiles_[4][4] = Wall1Tile;
	map->tiles_[4][3] = Wall1Tile;
	map->tiles_[4][2] = Wall1Tile;

	map->tiles_[5][9] = Wall1Tile;
	map->tiles_[6][9] = Wall1Tile;

	map->tiles_[9][11] = Wall1Tile;
	map->tiles_[9][10] = Wall1Tile;
	map->tiles_[11][6] = Wall1Tile;
	map->tiles_[10][5] = Wall1Tile;
	map->tiles_[10][4] = Wall1Tile;
	map->tiles_[10][3] = Wall1Tile;
	map->tiles_[10][2] = Wall1Tile;
	map->tiles_[10][1] = Wall1Tile;

	shared_ptr<Sprite> door_top(new Sprite(graphics, "PrtCave",
		units::tileToPixel(10), units::tileToPixel(0), units::tileToPixel(1), units::tileToPixel(1)));

	shared_ptr<Sprite> door_bottom(new Sprite(graphics, "PrtCave",
		units::tileToPixel(10), units::tileToPixel(1), units::tileToPixel(1), units::tileToPixel(1)));

	map->background_tiles_[4][18] = door_top;
	map->background_tiles_[5][18] = door_bottom;
	
	return map;
}

vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle& rectangle) const 
{
	const units::Tile first_row = units::gameToTile(rectangle.top());
	const units::Tile last_row = units::gameToTile(rectangle.bottom());
	const units::Tile first_col = units::gameToTile(rectangle.left());
	const units::Tile last_col = units::gameToTile(rectangle.right());
	
	vector<CollisionTile> collision_tiles;
	for (units::Tile row = first_row; row <= last_row; row++) 
	{
		for (units::Tile col = first_col; col <= last_col; col++) 
		{
			collision_tiles.push_back(CollisionTile(row, col, tiles_[row][col].tile_type));
		}
	}

	return collision_tiles;
}

void Map::update(units::MS elapsed_time_ms)
{
	for (size_t row = 0; row < tiles_.size(); row++) 
	{
		for (size_t col = 0; col < tiles_[row].size(); col++)
		{
			if (tiles_[row][col].sprite)
			{
				tiles_[row][col].sprite->update(elapsed_time_ms);
			}
		}
	}
}

void Map::drawBackground(Graphics& graphics) const 
{
	backdrop_->draw(graphics);

	for (size_t row = 0; row < background_tiles_.size(); row++) 
	{
		for (size_t col = 0; col < background_tiles_[row].size(); col++) 
		{
			if (background_tiles_[row][col])
			{
				background_tiles_[row][col]->draw(graphics, units::tileToGame(col), units::tileToGame(row));
			}
		}
	}
}

void Map::drawForeground(Graphics& graphics) const
{
	for (size_t row = 0; row < foreground_tiles_.size(); row++)
	{
		for (size_t col = 0; col < foreground_tiles_[row].size(); col++)
		{
			if (foreground_tiles_[row][col])
			{
				foreground_tiles_[row][col]->draw(graphics, units::tileToGame(col), units::tileToGame(row));
			}
		}
	}
}

void Map::draw(Graphics& graphics) const 
{
	for (size_t row = 0; row < tiles_.size(); row++) 
	{
		for (size_t col = 0; col < tiles_[row].size(); col++)
		{
			if (tiles_[row][col].sprite) 
			{
				tiles_[row][col].sprite->draw(graphics, units::tileToGame(col), units::tileToGame(row));
			}
		}
	}
}
