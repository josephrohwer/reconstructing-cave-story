#pragma once
#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SDL.h>
#include <map>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Graphics {
private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;
	typedef std::map<std::string, SDL_Texture*> SpriteMap;
	SpriteMap sprite_sheets_;
public:
	Graphics();
	~Graphics();
	typedef SDL_Texture* SurfaceID;
	SurfaceID loadImage(const std::string& file_name, bool black_is_transparent);
	void clear();
	void renderCopy(SurfaceID source,
		SDL_Rect* source_rectangle,
		SDL_Rect* destination_rectangle);
	void flip();
};

#endif // !GRAPHICS_H_
