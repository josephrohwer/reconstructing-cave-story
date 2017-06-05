#include "Map.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Game.h"
#include "Rectangle.h"

using boost::shared_ptr;
using std::vector;

// TODO: Implement map loading.

// STATIC
Map* Map::createTestMap(Graphics& graphics) {
	Map* map = new Map();

	map->backdrop_.reset(new FixedBackdrop("bkBlue", graphics));
	const int num_rows = 15; // 15 * 32 = 480; SCREEN HEIGHT
	const int num_cols = 20; // 20 * 32 = 640; SCREEN WIDTH

	// Ensure tiles_ and background_tiles_ are num_rows by num_cols in size.
	map->tiles_ = vector<vector<Tile>>(num_rows, vector<Tile>(num_cols, Tile()));
	map->background_tiles_ = vector<vector<shared_ptr<Sprite>>>(num_rows, vector<shared_ptr<Sprite>>(num_cols, shared_ptr<Sprite>()));

	shared_ptr<Sprite> sprite(new Sprite(graphics, "PrtCave", Game::kTileSize, 0, Game::kTileSize, Game::kTileSize));
	Tile tile(WALL_TILE, sprite);

	const int floor = 14;
	const int ceiling = 0;
	for (int col = 0; col < num_cols; col++) {
		map->tiles_[floor][col] = tile;
		map->tiles_[ceiling][col] = tile;
	}

	const int leftWall = 0;
	const int rightWall = 19;
	for (int row = 0; row < floor; row++) {
		map->tiles_[row][leftWall] = tile;
		map->tiles_[row][rightWall] = tile;
	}

	map->tiles_[6][18] = tile;
	map->tiles_[6][17] = tile;
	map->tiles_[6][16] = tile;
	map->tiles_[9][11] = tile;
	map->tiles_[9][10] = tile;
	map->tiles_[11][6] = tile;
	map->tiles_[10][5] = tile;
	map->tiles_[10][4] = tile;
	map->tiles_[10][3] = tile;
	map->tiles_[10][2] = tile;
	map->tiles_[10][1] = tile;

	shared_ptr<Sprite> chain_top(new Sprite(graphics, "PrtCave", 
		(11 * Game::kTileSize), (2 * Game::kTileSize), Game::kTileSize, Game::kTileSize));

	shared_ptr<Sprite> chain_middle(new Sprite(graphics, "PrtCave",
		(12 * Game::kTileSize), (2 * Game::kTileSize), Game::kTileSize, Game::kTileSize));

	shared_ptr<Sprite> chain_bottom(new Sprite(graphics, "PrtCave",
		(13 * Game::kTileSize), (2 * Game::kTileSize), Game::kTileSize, Game::kTileSize));

	map->background_tiles_[11][2] = chain_top;
	map->background_tiles_[12][2] = chain_middle;
	map->background_tiles_[13][2] = chain_bottom;
	
	return map;
}

vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle& rectangle) const {
	const int first_row = rectangle.top() / Game::kTileSize;
	const int last_row = rectangle.bottom() / Game::kTileSize;
	const int first_col = rectangle.left() / Game::kTileSize;
	const int last_col = rectangle.right() / Game::kTileSize;
	
	vector<CollisionTile> collision_tiles;
	for (int row = first_row; row <= last_row; row++) {
		for (int col = first_col; col <= last_col; col++) {
			collision_tiles.push_back(CollisionTile(row, col, tiles_[row][col].tile_type));
		}
	}
	return collision_tiles;
}

void Map::update(units::MS elapsed_time_ms) {
	for (size_t row = 0; row < tiles_.size(); row++) {
		for (size_t col = 0; col < tiles_[row].size(); col++) {
			if (tiles_[row][col].sprite) {
				tiles_[row][col].sprite->update(elapsed_time_ms);
			}
		}
	}
}

void Map::drawBackground(Graphics& graphics) const {
	backdrop_->draw(graphics);
	for (size_t row = 0; row < background_tiles_.size(); row++) {
		for (size_t col = 0; col < background_tiles_[row].size(); col++) {
			if (background_tiles_[row][col]) {
				background_tiles_[row][col]->draw(graphics, (col * Game::kTileSize), (row * Game::kTileSize));
			}
		}
	}
}

void Map::draw(Graphics& graphics) const {
	for (size_t row = 0; row < tiles_.size(); row++) {
		for (size_t col = 0; col < tiles_[row].size(); col++) {
			if (tiles_[row][col].sprite) {
				tiles_[row][col].sprite->draw(graphics, (col * Game::kTileSize), (row * Game::kTileSize));
			}
		}
	}
}