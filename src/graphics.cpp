#include <last-outpost/graphics.h>

namespace Game
{
    Graphics::Graphics()
        : tileWidth(0), tileHeight(0)
    {
    }

    void Graphics::setResolution(int screenWidth, int screenHeight, int tilesX, int tilesY)
    {
        int tileSize = std::min(screenWidth / tilesX, screenHeight / tilesY);

        tileWidth = tileSize;
        tileHeight = tileSize;
    }
    void Graphics::renderMap(SDL_Renderer *renderer, const Mylib::Matrix<Object> &terrain)
    {
        for (uint32_t row = 0; row < terrain.get_nrows(); ++row)
        {
            for (uint32_t col = 0; col < terrain.get_ncols(); ++col)
            {
                int x = col * tileWidth;
                int y = row * tileHeight;

                terrain[row, col].render(renderer, x, y, tileWidth, tileHeight);
            }
        }
    }
}