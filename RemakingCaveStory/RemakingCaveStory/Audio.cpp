#include "Audio.h"

Audio::Audio() {
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	main_music_ = Mix_LoadMUS("MainMusic.wav");
	jump_sound_ = Mix_LoadWAV("JumpSound.wav");
	damage_sound_ = Mix_LoadWAV("DamageSound.wav");
	bat_sound_ = Mix_LoadWAV("BatSound.wav");
}

Audio::~Audio() {
	Mix_FreeChunk(damage_sound_);
	Mix_FreeChunk(jump_sound_);
	Mix_FreeChunk(bat_sound_);
	Mix_FreeMusic(main_music_);
}