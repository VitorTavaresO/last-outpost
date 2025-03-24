#ifndef __LAST_OUTPOST_OBJECT_H__
#define __LAST_OUTPOST_OBJECT_H__

#include <SDL.h>
#include <last-outpost/graphics.h>

namespace Game
{
    class Object
    {
    private:
        SDL_Color color;

    public:
        Object();
        virtual ~Object() = default;

        virtual void update();
        virtual void render(Graphics &graphics, const SDL_Point &position, const SDL_Point &size) const;

        void setColor(const SDL_Color &color);
        SDL_Color getColor() const;
    };
}

#endif