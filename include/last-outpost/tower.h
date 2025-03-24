#ifndef __LAST_OUTPOST_TOWER_H__
#define __LAST_OUTPOST_TOWER_H__

#include <my-lib/matrix.h>
#include <last-outpost/object.h>
#include <last-outpost/projectil.h>
#include <last-outpost/collision.h>

namespace Game
{
    class Tower : public Object
    {
    private:
        int range;
        Projectil projectile;
        Collision collision;

    public:
        Tower();

        void update() override;
        void render(SDL_Renderer *renderer, int x, int y, int width, int height) const override;

        void setRange(int range);
        int getRange() const;

        void setProjectile(const Projectil &projectile);
        const Projectil &getProjectile() const;

        // void setCollision(const Collision &collision);
        // const Collision &getCollision() const;
        // bool checkCollision(const SDL_Rect &other) const;
    };
}

#endif