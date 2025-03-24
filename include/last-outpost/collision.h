#ifndef __LAST_OUTPOST_COLLISION_H__
#define __LAST_OUTPOST_COLLISION_H__

#include <my-lib/math-vector.h>

namespace Game
{
    class Collision
    {
    private:
        MyLib::Vector2 boundsPosition;
        MyLib::Vector2 boundsSize;

    public:
        Collision(float x = 0, float y = 0, float width = 0, float height = 0);
        ~Collision() = default;

        void setBounds(float x, float y, float width, float height);
        bool checkCollision(const Collision &other) const;
    };
}

#endif