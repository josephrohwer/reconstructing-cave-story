#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include "Sprite.h"
#include "Units.h"

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
	const units::MS frame_time_;
	const units::Frame num_frames_;
	units::Frame current_frame_;
	units::MS elapsed_time_;
};

#endif // !ANIMATEDSPRITE_H_
