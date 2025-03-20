#ifndef __LAST_OUTPOST_OBJECT_H__
#define __LAST_OUTPOST_OBJECT_H__

#include <SDL.h>

namespace Game
{

    class Object
    {
    public:
        Object();
        virtual ~Object() = default;

        virtual void update();
        virtual void render(SDL_Renderer *renderer, int cellWidth, int cellHeight);
    };

}

#endif