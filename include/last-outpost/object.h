#ifndef __LAST_OUTPOST_OBJECT_H__
#define __LAST_OUTPOST_OBJECT_H__

#include <SDL.h>

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
        virtual void render(SDL_Renderer *renderer, int x, int y, int width, int height) const;

        void setColor(const SDL_Color &color);
        SDL_Color getColor() const;
    };
}

#endif