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

    void Object::render(Graphics &graphics, const SDL_Point &position, const SDL_Point &size) const
    {
        graphics.drawRect(position, size, color);
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