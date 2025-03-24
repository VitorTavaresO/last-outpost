#include <last-outpost/graphics.h>
#include <last-outpost/types.h>

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

    void Graphics::drawRect(const Vector &position, const Vector &size, const SDL_Color &color) const
    {
        if (!this->renderer)
        {
            SDL_Log("Renderer not set in Graphics!");
            return;
        }

        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);

        SDL_Rect rect = {
            static_cast<int>(position.x * tileWidth),
            static_cast<int>(position.y * tileHeight),
            static_cast<int>(size.x * tileWidth),
            static_cast<int>(size.y * tileHeight)};

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