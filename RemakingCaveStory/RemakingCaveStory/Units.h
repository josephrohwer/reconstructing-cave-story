#ifndef UNITS_H_
#define UNITS_H_

#include <cmath>

namespace units
{
	typedef float Game; // Float for extra precision. Intrisic units of position.
	typedef int Pixel; // Integer for discrete units. Pixel values can be positive or negative.
	typedef unsigned int Tile; // Also discrete, but non-negative.
	typedef unsigned int Frame; // Discrete, non-negative.
	typedef float Degrees;

	typedef unsigned int MS; // Discrete Milliseconds.
	typedef unsigned int FPS; // Frames per Second (Hz or 1 / Second)

	typedef float Velocity; // Game / MS
	typedef float Acceleration; // Game / MS / MS
	typedef float AngularVelocity; // Degrees / MS

	namespace
	{
		const Game kTileSize = 32.0f;
		const double kPi = (atan(1) * 4);
	}

	inline double degreesToRadians(Degrees degrees)
	{
		return (degrees * kPi / 180.0f);
	}

	inline Pixel gameToPixel(Game game)
	{
		// TODO: Stop assuming 32x32.
		return Pixel(round(game)); // Divde game by 2 to make game 16x16 instead of 32x32.
	}

	inline Tile gameToTile(Game game)
	{
		return Tile(game / kTileSize);
	}

	inline Game tileToGame(Tile tile)
	{
		return (tile * kTileSize);
	}

	inline Pixel tileToPixel(Tile tile)
	{
		return gameToPixel(tileToGame(tile));
	}
}

#endif // !UNITS_H_
