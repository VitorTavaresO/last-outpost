#include <last-outpost/graphics.h>

namespace Game
{
    Graphics::Graphics()
        : tileWidth(0), tileHeight(0)
    {
    }

    void Graphics::setResolution(int screenWidth, int screenHeight, int tilesX, int tilesY)
    {
        tileWidth = screenWidth / tilesX;
        tileHeight = screenHeight / tilesY;
    }

    void Graphics::renderMap(SDL_Renderer *renderer, const Mylib::Matrix<SDL_Color> &colors)
    {
        for (uint32_t row = 0; row < colors.get_nrows(); ++row)
        {
            for (uint32_t col = 0; col < colors.get_ncols(); ++col)
            {
                SDL_Rect tile;
                tile.x = col * tileWidth;
                tile.y = row * tileHeight;
                tile.w = tileWidth;
                tile.h = tileHeight;

                SDL_Color color = colors[row, col];
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

                SDL_RenderFillRect(renderer, &tile);
            }
        }
    }
}