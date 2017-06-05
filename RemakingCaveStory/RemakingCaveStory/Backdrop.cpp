#include <SDL.h>
#include "Backdrop.h"
#include "Game.h"


namespace {
	const int kBackgroundSize = (Game::kTileSize * 4); // Pixels
}

FixedBackdrop::FixedBackdrop(const std::string& path, Graphics& graphics) {
	surface_id_ = graphics.loadImage(path);
}

void FixedBackdrop::draw(Graphics& graphics) const {
	for (int x = 0; x < Game::kScreenWidth; x += kBackgroundSize) {
		for (int y = 0; y < Game::kScreenHeight; y += kBackgroundSize) {
			SDL_Rect destination_rectangle;
			destination_rectangle.x = x;
			destination_rectangle.y = y;
			graphics.renderCopy(surface_id_, NULL, &destination_rectangle);
		}
	}
}