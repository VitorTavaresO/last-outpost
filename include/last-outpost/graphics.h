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
        Graphics();
        ~Graphics() = default;

        void setResolution(int screenWidth, int screenHeight, int tilesX, int tilesY);
        void setRenderer(SDL_Renderer *renderer);

        void drawRect(const Vector &position, const Vector &size, const SDL_Color &color) const;

        int getTileWidth() const;
        int getTileHeight() const;
    };
}

#endif