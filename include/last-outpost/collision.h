#ifndef __LAST_OUTPOST_COLLISION_H__
#define __LAST_OUTPOST_COLLISION_H__

#include <SDL.h>

namespace Game
{
    class Collision
    {
    private:
        SDL_Rect bounds;

    public:
        Collision(int x = 0, int y = 0, int width = 0, int height = 0);
        ~Collision() = default;

        void setBounds(int x, int y, int width, int height);
        const SDL_Rect &getBounds() const;

        bool checkCollision(const SDL_Rect &other) const;
    };
}

#endif