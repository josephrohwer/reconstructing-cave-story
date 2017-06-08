#ifndef BACKDROP_H_
#define BACKDROP_H_

#include <string>
#include "Graphics.h"

class Backdrop
{
public:
	virtual void draw(Graphics& graphics) const = 0;
private:
};

class FixedBackdrop : public Backdrop
{
public:
	FixedBackdrop(const std::string& path, Graphics& graphics);

	void draw(Graphics& graphics) const;
private:
	Graphics::SurfaceID surface_id_;
};

#endif // !BACKDROP_H_
