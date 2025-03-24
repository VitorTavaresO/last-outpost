#ifndef __LAST_OUTPOST_GRAPHICS_H__
#define __LAST_OUTPOST_GRAPHICS_H__

#include <SDL.h>
#include <my-lib/matrix.h>

namespace Game
{
    class Graphics
    {
    private:
        int tileWidth;
        int tileHeight;

    public:
        Graphics();
        ~Graphics() = default;

        void setResolution(int screenWidth, int screenHeight, int tilesX, int tilesY);

        void renderMap(SDL_Renderer *renderer, const Mylib::Matrix<SDL_Color> &colors);
    };
}

#endif