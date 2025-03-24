#include <last-outpost/graphics.h>
#include <algorithm>

namespace Game
{
    Graphics::Graphics(int screenWidth, int screenHeight, int tilesX, int tilesY, SDL_Renderer *renderer)
        : renderer(renderer)
    {
        if (!renderer)
        {
            SDL_Log("Renderer not provided to Graphics constructor!");
            throw std::runtime_error("Renderer is null");
        }

        int tileSize = std::min(screenWidth / tilesX, screenHeight / tilesY);
        tileWidth = tileSize;
        tileHeight = tileSize;
    }

    void Graphics::drawRect(const Vector &position, const Vector &size, const SDL_Color &color) const
    {
        if (!renderer)
        {
            SDL_Log("Renderer not set in Graphics!");
            return;
        }

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        SDL_Rect rect = {
            static_cast<int>(position.x * tileWidth),
            static_cast<int>(position.y * tileHeight),
            static_cast<int>(size.x * tileWidth),
            static_cast<int>(size.y * tileHeight)};

        SDL_RenderFillRect(renderer, &rect);
    }
}