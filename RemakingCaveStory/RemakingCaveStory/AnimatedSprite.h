#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include "Sprite.h"
#include "Units.h"
#include "Timer.h"

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(Graphics& graphics,
				   const std::string& file_name,
				   units::Pixel source_x, units::Pixel source_y,
				   units::Pixel width, units::Pixel height,
				   units::FPS fps, units::Frame num_frames);

	void update(units::MS elapsed_time);
private:
	Timer frame_timer_;
	const units::Frame num_frames_;
	units::Frame current_frame_;
};

#endif // !ANIMATEDSPRITE_H_
