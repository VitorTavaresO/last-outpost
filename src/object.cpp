#include <last-outpost/object.h>
#include <last-outpost/types.h>

namespace Game
{
    Object::Object()
        : color({0, 0, 0, 255}), position({0, 0}), size({1, 1})
    {
    }

    void Object::update()
    {
    }

    void Object::render(Graphics &graphics) const
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

    void Object::setPosition(float x, float y)
    {
        position = {x, y};
    }

    Vector Object::getPosition() const
    {
        return position;
    }

    void Object::setSize(float width, float height)
    {
        size = {width, height};
    }

    Vector Object::getSize() const
    {
        return size;
    }
}