#include <last-outpost/object.h>

namespace Game
{
    Object::Object()
        : color({0, 0, 0, 255})
    {
    }

    void Object::update()
    {
    }

    void Object::render(SDL_Renderer *renderer, int x, int y, int width, int height) const
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        SDL_Rect rect = {x, y, width, height};
        SDL_RenderFillRect(renderer, &rect);
    }

    void Object::setColor(const SDL_Color &color)
    {
        this->color = color;
    }

    SDL_Color Object::getColor() const
    {
        return color;
    }
}