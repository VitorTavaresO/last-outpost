#ifndef __LAST_OUTPOST_GLOBALS_H__
#define __LAST_OUTPOST_GLOBALS_H__

#include <SDL.h>

namespace Game
{
	inline int SCREEN_WIDTH = 1920;
	inline int SCREEN_HEIGHT = 1080;
	inline int TILES_X = 32;
	inline int TILES_Y = 18;

	inline float getTimeInSeconds()
	{
		return static_cast<float>(SDL_GetTicks()) / 1000.0f;
	}
}

#endif