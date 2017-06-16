#ifndef AUDIO_H_
#define AUDIO_H_

#include <SDL_mixer.h>

class Audio {
public:
	Audio();
	~Audio();

	Mix_Music *main_music_;
	Mix_Chunk *jump_sound_;
	Mix_Chunk *damage_sound_;
	Mix_Chunk *bat_sound_;
private:
};
#endif // !AUDIO_H_
