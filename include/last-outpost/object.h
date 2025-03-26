#ifndef __LAST_OUTPOST_OBJECT_H__
#define __LAST_OUTPOST_OBJECT_H__

#include <SDL.h>
#include <last-outpost/graphics.h>
#include <last-outpost/types.h>

namespace Game
{
    class Object
    {
    private:
        SDL_Color color;
        Vector position;
        Vector size;

    public:
        Object();
        virtual ~Object() = default;

        virtual void update();
        virtual void render(Graphics &graphics) const;

        void setColor(const SDL_Color &color)
        {
            this->color = color;
        }

        SDL_Color getColor() const
        {
            return color;
        }

        void setPosition(float x, float y)
        {
            this->position = {x, y};
        }

        Vector getPosition() const
        {
            return position;
        }

        void setSize(float width, float height)
        {
            this->size = {width, height};
        }

        Vector getSize() const
        {
            return size;
        }
    };
}

#endif