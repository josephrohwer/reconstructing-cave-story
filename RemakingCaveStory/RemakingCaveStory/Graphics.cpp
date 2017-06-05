#include <string>
#include "Graphics.h"
#include "Game.h"

Graphics::Graphics() {
	window_ = SDL_CreateWindow("Cave Story",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		Game::kScreenWidth, Game::kScreenHeight, SDL_WINDOW_RESIZABLE);
	renderer_ = SDL_CreateRenderer(window_, -1, 0);
	SDL_RenderSetLogicalSize(renderer_, Game::kScreenWidth, Game::kScreenHeight);
	SDL_ShowCursor(SDL_DISABLE);
}

Graphics::~Graphics() {
	for (SpriteMap::iterator it = sprite_sheets_.begin(); it != sprite_sheets_.end(); it++) {
		SDL_DestroyTexture(it->second);
	}
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
}

Graphics::SurfaceID Graphics::loadImage(const std::string& file_name, bool black_is_transparent) {
	const std::string file_path = file_name + ".bmp";
	// if we have not loaded in the spritesheet
	if (sprite_sheets_.count(file_path) == 0) {
		// load it in now
		SDL_Surface* image = SDL_LoadBMP(file_path.c_str());
		if (!image) {
			exit(EXIT_FAILURE);
		}
		if (black_is_transparent) {
			const Uint32 black_color = SDL_MapRGB(image->format, 0, 0, 0);
			SDL_SetColorKey(image, SDL_TRUE, black_color);
		}
		sprite_sheets_[file_path] = SDL_CreateTextureFromSurface(renderer_, image);
		SDL_FreeSurface(image);
	}
	return sprite_sheets_[file_path];
}

void Graphics::clear() {
	SDL_RenderClear(renderer_);
}

void Graphics::renderCopy(
	SurfaceID source,
	SDL_Rect* source_rectangle,
	SDL_Rect* destination_rectangle) {
	if (source_rectangle) {
		destination_rectangle->w = source_rectangle->w;
		destination_rectangle->h = source_rectangle->h;
	}
	else {
		uint32_t format;
		int access, w, h;
		SDL_QueryTexture(source, &format, &access, &w, &h);
		destination_rectangle->w = w;
		destination_rectangle->h = h;
	}
	SDL_RenderCopy(renderer_, source, source_rectangle, destination_rectangle);
}

void Graphics::flip() {
	// Update the screen with any rendering performed since the previous call.
	SDL_RenderPresent(renderer_);
}
