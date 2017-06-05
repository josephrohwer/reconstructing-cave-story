#pragma once
#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include "Sprite.h"
#include "Units.h"

// AnimatedSprite is a Sprite so use inheritance here.
class AnimatedSprite : public Sprite {
private:
	const units::MS frame_time_;
	const int num_frames_;
	int current_frame_;
	units::MS elapsed_time_; // Elapsed time since the last frame change.
public:
	AnimatedSprite(Graphics& graphics,
		const std::string& file_name,
		int source_x, int source_y,
		int width, int height,
		units::FPS fps, int num_frames);
	void update(units::MS elapsed_time_ms);
};

#endif // !ANIMATEDSPRITE_H_
