#include <last-outpost/graphics.h>

namespace Game
{
    Graphics::Graphics()
        : tileWidth(0), tileHeight(0), renderer(nullptr)
    {
    }

    void Graphics::setResolution(int screenWidth, int screenHeight, int tilesX, int tilesY)
    {
        int tileSize = std::min(screenWidth / tilesX, screenHeight / tilesY);
        tileWidth = tileSize;
        tileHeight = tileSize;
    }

    void Graphics::setRenderer(SDL_Renderer *renderer)
    {
        this->renderer = renderer;
    }

    void Graphics::drawRect(const SDL_Point &position, const SDL_Point &size, const SDL_Color &color) const
    {
        if (!renderer)
        {
            SDL_Log("Renderer not set in Graphics!");
            return;
        }

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        SDL_Rect rect = {
            position.x * tileWidth,
            position.y * tileHeight,
            size.x * tileWidth,
            size.y * tileHeight};

        SDL_RenderFillRect(renderer, &rect);
    }

    int Graphics::getTileWidth() const
    {
        return tileWidth;
    }

    int Graphics::getTileHeight() const
    {
        return tileHeight;
    }
}