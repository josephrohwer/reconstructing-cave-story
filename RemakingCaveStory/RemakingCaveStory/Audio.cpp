#include "Audio.h"

Audio::Audio() {
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	main_music_ = Mix_LoadMUS("MainMusic.wav");
	jump_sound_ = Mix_LoadWAV("JumpSound.wav");
}

Audio::~Audio() {
	Mix_FreeChunk(jump_sound_);
	Mix_FreeMusic(main_music_);
}