#ifndef __LAST_OUTPOST_GRAPHICS_H__
#define __LAST_OUTPOST_GRAPHICS_H__

#include <SDL.h>
#include <last-outpost/types.h>

namespace Game
{
	class Graphics
	{
	private:
		int tileWidth;
		int tileHeight;
		SDL_Renderer *renderer;

	public:
		Graphics(int screenWidth, int screenHeight, int tilesX, int tilesY, SDL_Renderer *renderer);

		int getTileWidth() const
		{
			return this->tileWidth;
		}

		int getTileHeight() const
		{
			return this->tileHeight;
		}

		void drawRect(const Vector &position, const Vector &size, const SDL_Color &color) const;
	};
}

#endif