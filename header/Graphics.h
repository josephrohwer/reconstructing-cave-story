#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SDL.h>
#include <map>
#include <string>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Graphics
{
public:
	Graphics();
	~Graphics();
	void clear();
	typedef SDL_Texture* SurfaceID;
	SurfaceID loadImage(const std::string& file_name, bool black_is_transparent = false);
	void renderCopy(SurfaceID source,
				    SDL_Rect* source_rectangle,
					SDL_Rect* destination_rectangle);
	void flip();
private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;
	typedef std::map<std::string, SDL_Texture*> SpriteMap;
	SpriteMap sprite_sheets_;
};

#endif // !GRAPHICS_H_
