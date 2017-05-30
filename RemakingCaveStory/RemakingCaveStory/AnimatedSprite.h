#pragma once
#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include "Sprite.h"

// AnimatedSprite is a Sprite so use inheritance here.
class AnimatedSprite : public Sprite {
private:
	const int frame_time_;
	const int num_frames_;
	int current_frame_;
	int elapsed_time_; // Elapsed time since the last frame change.
public:
	AnimatedSprite(Graphics& graphics,
		const std::string& file_name,
		int source_x, int source_y,
		int width, int height,
		int fps, int num_frames);
	void update(int elapsed_time_ms);
};

#endif // !ANIMATEDSPRITE_H_
